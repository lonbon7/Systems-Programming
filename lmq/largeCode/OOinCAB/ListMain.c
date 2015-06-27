#include <stdio.h>
#include "SmartAlloc.h"
#include "List.h"

const int NUM_LISTS = 3;

int main() {
   List *lists[NUM_LISTS];
     int list;
   ListAddType where;
   char command;
   double value;
   
   for (list = 0; list < NUM_LISTS; list++)
      lists[list] = ListCreate();
      
    while (EOF != scanf(" %c %d", &command, &list))
      if (command == 'a') {
         scanf("%lf %d", &value, &where);
         ListAdd(lists[list], value, where);
      }
      else if (command == 'd') {
         scanf("%lf", &value);
         ListDrop(lists[list], value);
      }
      else if (command == 'p') {
         printf("List %d has %d elements: ", list, ListGetCount(lists[list]));
         for (ListStart(lists[list]); ListHasNext(lists[list]);
          ListAdvance(lists[list]))
            printf("%0.3f ", ListCurrent(lists[list]));
         printf("\n");
      }
      
   for (list = 0; list < NUM_LISTS; list++)
      ListDestroy(lists[list]);
      
   printf("Final space: %d\n", report_space());
}