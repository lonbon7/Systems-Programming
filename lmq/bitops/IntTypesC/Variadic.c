#include <stdio.h>

int Sum(int value, ...) {
   int sum = 0, *pointer = &value;
   //char *pointer = &value
  printf("Initial pointer: %d\n", *pointer);
  
   while(*pointer) {
      sum += *pointer;
      pointer += 1;
      printf("Value in loop: %d\n", *pointer);
   }

   return sum;
}

int main() {
   printf("%d %d %d %d\n", Sum(0), Sum(3, 5, 6, 7, 0),
    Sum(7, 2, 42, 3, 5, -4, 0), Sum(-1, 9, 12, 123, -213, 42, 7, 2, 0));
}