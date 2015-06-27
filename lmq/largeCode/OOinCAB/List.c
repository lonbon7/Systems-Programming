#include <stdlib.h>
#include <assert.h>
#include "SmartAlloc.h"
#include "List.h"

struct List {
   struct List *head;
   struct List *at;
   int count;
};

typedef struct Node {
   double data;
   struct Node *next;
} Node;

List *ListCreate() {
   return calloc(sizeof(List), 1);
}

void ListDestroy(List **list) {
   Node *temp;
   
   while ((*list)->head) {
      temp = (*list)->head;
      (*list)->head = temp->next;
      free(temp);
   }
}

int ListGetCount(List *list) {
   return list->count;
}

void ListAdd(List *list, double toAdd, ListAddType where) {
   Node *last, *temp = malloc(sizeof(Node));

   temp->data = toAdd;
   list->count++;   
   if (where == LIST_AT_START) {
      temp->next = list->head;
      list->head = temp;
   }
   else {
      for (last = list->head; last && last->next; last = last->next)
	      ;
      if (last)
	      last->next = temp;
      else
         list->head = temp;
	   temp->next = NULL;
   }
}
   
void ListDrop(List *list, double toDrop) {
   Node *prior = NULL, *temp;
   
   for (temp = list->head; temp && temp->data != toDrop; temp = temp->next)
      prior = temp;
   if (temp) {
      if (prior)
	      prior->next = temp->next;
      else
	      list->head = temp->next;
	   free(temp);
      list->count--;
   }
}

void ListStart(List *list) {
   list->at = list->head;
}

int ListHasNext(List *list) {
   return list->at != NULL;
}

// Call this only if ListHasNext returns true.
double ListCurrent(List *list) {
   assert(list->at != NULL);
   return list->at->data;
}

// Call this only if ListHasNext returns true.
void ListAdvance(List *list) {
   assert(list->at != NULL);
   list->at = list->at->next;
}