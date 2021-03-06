#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define DIM1 6
#define DIM2 8
#define NAME_LEN 20

#define qsort NOT_ALLOWED


typedef struct Name {
   char first[NAME_LEN+1];
   char last[NAME_LEN+1];
} Name;

// Support functions NameLess, NameGreater, DblLess, and StrLess
int NameLess(char *first, char *second) {
   int result;
   Name *nameOne = (Name *)first;
   Name *nameTwo = (Name *)second;

   result = strcmp(nameOne->last, nameTwo->last);
   if (!result) 
      result = strcmp(nameOne->first, nameTwo->first);      
   
   return result < 0 ? 1 : 0;
}

int NameGreater(char *first, char *second) {  
   return NameLess(first, second) > 0 ? 0 : 1;   
}

int DblLess(char *first, char *second) {  
   return *(double *)first < *(double *)second;
}

int StrLess(char *first, char *second) {   
   return strcmp(*(char **)first, *(char **)second) < 0 ? 1 : 0;
}

// Function GenInsertSort
void GenInsertSort(void *vals, int arraySize, int elementSize, int (*rule)(char *, char *)) {
   int ndx; 
   char *array = vals, *insNdx, *toInsert = malloc(elementSize);  
   
   for (ndx = 1; ndx < arraySize; ndx++) { 
      
      insNdx = array + ndx * elementSize; 
      toInsert = memcpy(toInsert, insNdx, elementSize);
      
      while (insNdx > array && (*rule)(toInsert, insNdx - elementSize)) {          
         memcpy(insNdx, insNdx - elementSize, elementSize);        
         insNdx -= elementSize;         
      }
     memcpy(insNdx, toInsert, elementSize);
   }
}

void main() {
   Name names[DIM1] = {{"John", "Smith"}, {"Jane", "Smith"}, {"Bill", "Jones"},
      {"Sue", "Johnson"}, {"Susan", "Johnson"}, {"Jim", "Taylor"}
   };
   double vals[DIM2] = {1.1, -2.2, 3.3, -4.2, 5.5, -6.6, 7.7, -8.8};
   char *words[DIM2]
    = {"alpha", "beta", "gamma", "delta", "epsilon", "pi", "phi", "omega"};
   int i;
   
   for (i = 0; i < DIM1; i++)
      printf("%s %s\n", names[i].first, names[i].last);
   printf("\n");
      
   GenInsertSort(names, DIM1, sizeof(Name), NameLess);
   for (i = 0; i < DIM1; i++)
      printf("%s %s\n", names[i].first, names[i].last);
	  
   GenInsertSort(names, DIM1, sizeof(Name), NameGreater);   
   for (i = 0; i < DIM1; i++)
      printf("%s %s\n", names[i].first, names[i].last);

   GenInsertSort(vals, DIM2, sizeof(double), DblLess);
   for (i = 0; i < DIM2; i++)
      printf("%f ", vals[i]);
   printf("\n");
   
   GenInsertSort(words, DIM2, sizeof(char *), StrLess);
   for (i = 0; i < DIM2; i++)
      printf("%s ", words[i]);
   printf("\n");
}