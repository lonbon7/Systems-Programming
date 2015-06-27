#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "SmartAlloc.h"

void printArgs(char **args) {
   while (*args)
      printf("%s\n", *args++);
}

void burnArgs(char **args, int argc) {
   while (argc--) {
      **args = 'X';
      *args++ = NULL;
   }
}

void copyArgs(int argc, char ***dupArgs, char **argv) {
   *dupArgs = calloc(argc, sizeof(char *));
      
   while (*++argv) {
     // printf("Made it inside the while loop\n");
      **dupArgs = calloc(strlen(*argv) + 1, sizeof(char));
    //  printf("String length: %d\n", strlen(*argv));
     // printf("String: %s\n", *argv);
    //  printf("Made it past the calloc call\n");
      **dupArgs = strcpy(**dupArgs, *argv);
      (*dupArgs)++;
      //printf("Made it past the strcpy call\n");
   }
}

void main(int argc, char **argv) {
   char **dupArgs;
   
   copyArgs(argc, &dupArgs, argv);   
   burnArgs(argv, argc);
   printArgs(dupArgs);
   printf("Space: %d\n", report_space());
}