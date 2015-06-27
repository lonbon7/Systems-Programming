#include <stdio.h>
#include <stdlib.h>

typedef struct Node {
   int data;          // Could put any kind of data here
   struct Node *next; // Self-referential requires "struct"
} Node;

Node *AddAtEnd(Node *head, int data) {
   Node *node, *temp;
   
   node = calloc(sizeof(Node), 1);
   node->data = data;    

   if(head) {
      for (temp = head; temp->next; temp = temp->next)
         ;
      temp->next = node;
   }
   else
      head = node;
   return head;
}

void PrintAll(char *tag, Node *head) {
   Node *temp;
   
   printf("%s:", tag);
   for (temp = head; temp != NULL; temp = temp->next)
      printf(" %d", temp->data);
   printf("\n");
}

int main() {
   Node *head = NULL;
   
   head = AddAtEnd(head, 40);
   head = AddAtEnd(head, 30);
   head = AddAtEnd(head, 20);
   head = AddAtEnd(head, 50);
   head = AddAtEnd(head, 10);
   
   PrintAll("After insert", head);
}
   