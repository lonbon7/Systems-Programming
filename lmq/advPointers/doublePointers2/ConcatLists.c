#include <stdio.h>
#include <stdlib.h>

typedef struct Node {
   int data;
   struct Node *next; 
} Node;

void ReadList(Node **pHead) {
   int data;
   Node *nd, *temp;  // You may use temp, but don't have to.

   *pHead = NULL;
   temp = *pHead;

   do {
     // printf("Made it into the while loop in ReadList\n");
      scanf("%d", &data);
      if (data) {
         nd = calloc(sizeof(Node), 1);
         nd->data = data;
         if (*pHead)
            temp->next = nd;      
         else
            *pHead = nd;
         temp = nd;
      }
   } while (data);
   //printf("Made it out of the while loop in ReadList\n");
}

void PrintList(Node *head) {
   while (head) {
      printf("%d ", head->data);
	  head = head->next;
   }
   printf("\n");
}

void ConcatLists(Node **head1, Node **head2) {
   Node *temp; // You may use this, but you don't have to.
   temp = *head1;
   if (temp) {
      while (temp->next)
         temp = temp->next;
      temp->next = *head2;
   }
   else
      *head1 = *head2;   
   *head2 = NULL;
}

int main() {
   Node *head1, *head2;

   ReadList(&head1);
   ReadList(&head2);
   printf("Made it past ReadList calls in main\n");
   ConcatLists(&head1, &head2);
   printf("Made it past ConcatLists call in main\n");
   PrintList(head1);
   PrintList(head2);
   
   return 0;
}