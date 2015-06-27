#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define IDIM1 10
#define IDIM2 4

int * insert(int *blk, int *toInsert, int dim1, int dim2, int offset) {
   int *oldblk = blk, *cursor, newdim = dim1 - offset;
   blk = calloc(sizeof(int), dim1 + dim2);
   cursor = blk;

   while (offset--)
      *cursor++ = *oldblk++;     
   while (dim2--)
      *cursor++ = *toInsert++;
   while (newdim --)
      *cursor++ = *oldblk++;
   
   return blk;
}    



void scanInts(int *arr, int size) {
   int i;
   
   for (i = 0; i < size; i++)
      scanf("%d", arr+i);
}

void printInts(char *name, int *arr, int size) {
   int i;
   
   printf("%s:", name);
   for (i = 0; i < size; i++)
      printf(" %d", arr[i]);
   printf("\n");
}

int main() {
   int *iArr1 = malloc(sizeof(int) * IDIM1);
   int *iArr2 = malloc(sizeof(int) * IDIM2);
   
   scanInts(iArr1, IDIM1);
   scanInts(iArr2, IDIM2);
   iArr1 = insert(iArr1, iArr2, IDIM1, IDIM2, IDIM1/2);
  // printf("Made it past the first insert() call in main()\n");
   iArr2 = insert(iArr2, iArr1, IDIM2, IDIM1 + IDIM2, IDIM2/2);
   //printf("Made it past the second insert() call in main()\n");

   printInts("iArr1", iArr1, IDIM1 + IDIM2);
   printInts("iArr2", iArr2, IDIM1 + 2*IDIM2);
   
   free(iArr1);
   free(iArr2);
}