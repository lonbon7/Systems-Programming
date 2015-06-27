#include <stdio.h>
#include "List.h"
#include "SmartAlloc.h"

int main() {
   List **lists;
   int numLists, list;
   char command;
   double value;
   
   scanf("%d", &numLists); // Find out how many lists we want
   lists = calloc(sizeof(List *), numLists);
   
   // Create lists, and put a sample value on each
   for (list = 0; list < numLists; list++) {
      lists[list] = ListCreate();
      ListAdd(lists[list], 42.0, LIST_AT_START);
   }
   printf("Made it past ListAdd and ListCreate\n");
  // ListPrintList();
   // Hand-delete selected ones, but not all
   while (EOF != scanf("%d", &list)) {
      numLists--;
      ListDestroy(lists[list]);
   }
   printf("Made it past ListDestroy\n");
   if (numLists)
      ListDestroyAll();    // Destroy all still-existing lists
   printf("Made it past ListDestroyAll\n");
   ListClearFreelist(); // Burn the Node freelist
   free(lists);         // Free our list pointer block
   
   printf("%d bytes left\n", report_space());  // Should be 0
   
   return 0;
}

