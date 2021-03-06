#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>

#include "LZWCmp.h"
#include "SmartAlloc.h"

static void *InitCodeSet();
static void AddNode(TreeNode *, TreeNode *, void *);
static void InitBST(LZWCmp *);
static UInt FindCode(LZWCmp *, TreeNode *);
static void DumpTree(TreeNode *, void *);
static void DestructTree(TreeNode *);
static int CodeCmp(Code, Code);
static void NewNode(TreeNode **, int);
static void PrintCode(Code);
static void CodeCpy(UChar *, UChar *, int);
static void PackBits(LZWCmp *, UInt, int);
static void BumpPCode(LZWCmp *);
static void Recycle(LZWCmp *);

static TreeNode *treeList = NULL;

void LZWCmpInit(LZWCmp *cmp, CodeSink sink, void *sinkState, int recycleCode,
 int traceFlags) {
   cmp->root = NULL;
   cmp->sink = sink;
   cmp->sinkState = sinkState;
   cmp->numBits = INIT_NUM_BITS; 
   cmp->maxCode = INIT_MAX_CODE; 
   cmp->recycleCode = recycleCode; 
   cmp->nextInt = 0; 
   cmp->bitsUsed = 0; 
   cmp->cst = InitCodeSet();   
   InitBST(cmp);
   cmp->curCode.size = NUM_SYMS + 1;
   cmp->pCode.data = malloc(sizeof(char) * SIZE_INCR);   
   cmp->pCode.size = 0;
   cmp->pCodeLimit = SIZE_INCR;
   cmp->traceFlags = traceFlags;
   cmp->curLoc = cmp->root;         
 }

void LZWCmpEncode(LZWCmp *cmp, UChar sym) { 
   int index;
   State *state = cmp->sinkState;
   UInt code;
   static int count = 0;
     
   if (cmp->pCode.size == cmp->pCodeLimit)
      BumpPCode(cmp);   
 
   cmp->pCode.data[cmp->pCode.size++] = sym;  
   code = FindCode(cmp, cmp->curLoc); 
   
   printf("Curloc code: %d\n", cmp->curLoc->cNum);
  /* count++;
   if (count > 20)
      return;      
   PrintCode(cmp->pCode);
   printf("\n");*/
   
   if (code != FOUND_CODE) {      
      PackBits(cmp, code, false);   
      
      if (cmp->traceFlags & TRACE_CODES)
         printf("Sending code %d\n", code);
            
      if (cmp->traceFlags & TRACE_TREE) {
            DumpTree(cmp->root, cmp->cst);
            printf("|\n\n");
         }         
      cmp->pCode.data[0] = sym;
      cmp->pCode.size = 1;      
      FindCode(cmp, cmp->root);     
   }
   
   if (cmp->curCode.size > cmp->recycleCode)
      Recycle(cmp);
   
    if (cmp->curCode.size - 1 > cmp->maxCode 
     && cmp->curCode.size != cmp->recycleCode) {
      cmp->numBits++;    
      cmp->maxCode = cmp->maxCode * 2 + 1;     
      if (cmp->traceFlags & TRACE_BUMPS)
         printf("Bump numBits to %d\n", cmp->numBits);
      
   }  
}

void LZWCmpStop(LZWCmp *cmp) {
   State *state = cmp->sinkState;          
  
   if (cmp->pCode.size == 0)
      PackBits(cmp, EOD_CODE, true);
   else {
      PackBits(cmp, cmp->curLoc->cNum, false);       
      PackBits(cmp, EOD_CODE, true);      
      
      if (cmp->traceFlags & TRACE_CODES)
         printf("Sending code %d\n", cmp->curLoc->cNum);         
            
      if (cmp->traceFlags & TRACE_TREE) {
         DumpTree(cmp->root, cmp->cst);
         printf("|\n\n");   
      }      
   } 
  
   if (cmp->traceFlags & TRACE_CODES)
      printf("Sending code %d\n", EOD_CODE);
         
   if (cmp->traceFlags & TRACE_TREE) {
      DumpTree(cmp->root, cmp->cst);
      printf("|\n\n");
   }
  
   state->end = true;   
   cmp->sink(state, EOD_CODE, state->end);   
}   

void LZWCmpDestruct(LZWCmp *cmp) {   
   DestroyCodeSet(cmp->cst);  
   free(cmp->pCode.data);  
   DestructTree(cmp->root);  
   LZWCmpClearFreelist();   
}

void LZWCmpClearFreelist() {
   TreeNode *temp;
   static int count = 1;   
   
   while (treeList) {
      temp = treeList;
      treeList = treeList->right;      
      free(temp);      
   }
}

static void *InitCodeSet() {
   int numCodes;
   void *set = CreateCodeSet(RECYCLE_CODE + 1);
   
   for (numCodes = 0; numCodes <= NUM_SYMS; numCodes++)
      NewCode(set, numCodes);
   
   return set;
}

static void AddNode(TreeNode *root, TreeNode *newNode, void *codeSet) {     
   assert(root);
   
   if (CodeCmp(GetCode(codeSet, newNode->cNum),
    GetCode(codeSet, root->cNum)) > 0) {
      FreeCode(codeSet, newNode->cNum);
      FreeCode(codeSet, root->cNum);
      
      if (root->right)
         AddNode(root->right, newNode, codeSet);
      else
         root->right = newNode;
   }
   else {
      FreeCode(codeSet, newNode->cNum);
      FreeCode(codeSet, root->cNum);
      
      if (root->left)
         AddNode(root->left, newNode, codeSet);
      else
         root->left = newNode;
   }   
}

static void InitBST(LZWCmp *cmp) {
   int numCodes;
   TreeNode *node;
   
   for (numCodes = 0; numCodes < NUM_SYMS; numCodes++) {
      NewNode(&node, numCodes);
      
      if (cmp->root)
         AddNode(cmp->root, node, cmp->cst);
      else
         cmp->root = node;      
   }   
}

static UInt FindCode(LZWCmp *cmp, TreeNode *root) {   
   int result;
   
   if (CodeCmp(cmp->pCode, GetCode(cmp->cst, root->cNum)) == 0) {
      FreeCode(cmp->cst, root->cNum);
      cmp->curLoc = root;      
      
      return FOUND_CODE;
   }
   result = CodeCmp(cmp->pCode, GetCode(cmp->cst, root->cNum));
   printf("Less than result: %d\n", result);
   if (CodeCmp(cmp->pCode, GetCode(cmp->cst, root->cNum)) < 0) {  
      FreeCode(cmp->cst, root->cNum);
      FreeCode(cmp->cst, root->cNum);
     
      if (root->left == NULL) {         
         NewNode(&(root->left), cmp->curCode.size);        
         SetSuffix(cmp->cst, ExtendCode(cmp->cst, cmp->curLoc->cNum),
          cmp->pCode.data[cmp->pCode.size - 1]);
          cmp->curCode.size++;          
         
         return cmp->curLoc->cNum; 
      }
                     
      return FindCode(cmp, root->left);
   }
   result = CodeCmp(cmp->pCode, GetCode(cmp->cst, root->cNum));
   printf("Greater than result: %d\n", result);
   
      FreeCode(cmp->cst, root->cNum);
      FreeCode(cmp->cst, root->cNum);
     
      if (root->right == NULL) {         
         NewNode(&(root->right), cmp->curCode.size);              
         SetSuffix(cmp->cst, ExtendCode(cmp->cst, cmp->curLoc->cNum),
          cmp->pCode.data[cmp->pCode.size - 1]);
          cmp->curCode.size++;
         
         return cmp->curLoc->cNum;
      }
      
      return FindCode(cmp, root->right);
   
}

static void DumpTree(TreeNode *root, void *cst) {
   int index;      
 
   if (root) {      
      DumpTree(root->left, cst);
      PrintCode(GetCode(cst, root->cNum));
      FreeCode(cst, root->cNum);
      DumpTree(root->right, cst);
   }   
}

static void DestructTree(TreeNode *root) {  
   if (root) {     
      DestructTree(root->left);      
      DestructTree(root->right);  
      root->right = treeList;      
      treeList = root;      
   }
}

static int CodeCmp(Code pCode, Code treeCode) {
   int pSize, treeSize;
   
   for (pSize = 0, treeSize = 0; pSize < pCode.size && treeSize < treeCode.size
    && pCode.data[pSize] == treeCode.data[treeSize]; pSize++, treeSize++)
      ;
   
   if (pSize == pCode.size)
      pSize = 0;
   else
      pSize = pCode.data[pSize];
   if (treeSize == treeCode.size)
      treeSize = 0;
   else
      treeSize = treeCode.data[treeSize];
  
   return pSize - treeSize;
}

static void NewNode(TreeNode **node, int cNum) {  
   if (treeList) {
      *node = treeList;
      treeList = treeList->right;
      (*node)->right = (*node)->left = NULL;     
   }
   else
      *node = calloc(sizeof(TreeNode), 1);
   (*node)->cNum = cNum;   
}     

static void PrintCode(Code code) {
   int index;
   printf("|");
   for (index = 0; index < code.size; index++) {
      if (index)
         printf(" ");
      printf("%d", code.data[index]);   
   }
}

static void CodeCpy(UChar *dest, UChar *src, int size) {
   int index;
   
   for (index = 0; index < size; index++)
      dest[index] = src[index];   
}

static void BumpPCode(LZWCmp *cmp) {
   UChar *temp;

   temp = cmp->pCode.data;
   cmp->pCode.data = malloc(cmp->pCodeLimit + SIZE_INCR);
   CodeCpy(cmp->pCode.data, temp, cmp->pCode.size);
   cmp->pCodeLimit += SIZE_INCR;
   free(temp);
}

static void PackBits(LZWCmp *cmp, UInt code, int end) { 
   State *state = cmp->sinkState;   

   if (BITS_PER_INT - cmp->bitsUsed >= cmp->numBits) {             
      cmp->nextInt |= code << BITS_PER_INT - cmp->bitsUsed - cmp->numBits;      
      cmp->bitsUsed += cmp->numBits;
      
      if (cmp->bitsUsed == BITS_PER_INT || end) {
         cmp->sink(state, cmp->nextInt, state->end);
         cmp->nextInt = 0;
         cmp->bitsUsed = 0;
      }  
   }
   else {
      cmp->nextInt |= code >> cmp->numBits - (BITS_PER_INT - cmp->bitsUsed);
      cmp->sink(state, cmp->nextInt, state->end);
      cmp->nextInt = 0;
      cmp->nextInt |= code << BITS_PER_INT - 
       (cmp->numBits - (BITS_PER_INT - cmp->bitsUsed));     
      cmp->bitsUsed = cmp->numBits - (BITS_PER_INT - cmp->bitsUsed);
      
      if (end)
         cmp->sink(state, cmp->nextInt, state->end);
   }  
}
   
static void Recycle(LZWCmp *cmp) {   
   printf("Recycling dictionary...\n");
   DestroyCodeSet(cmp->cst);
   DestructTree(cmp->root);   
   cmp->curCode.size = NUM_SYMS + 1;
   cmp->root = NULL;
   InitCodeSet();
   InitBST(cmp);   
   cmp->numBits = INIT_NUM_BITS;
   cmp->maxCode = INIT_MAX_CODE;
   FindCode(cmp, cmp->root);  
}
