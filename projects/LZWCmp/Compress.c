#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "SmartAlloc.h"
#include "LZWCmp.h"

static void ProcessCodes(void *, UInt, int);
static void StateInit(State *, char *);
static void StateStop(State *);

int main(int argc, char *argv[]) {
   LZWCmp cmp;   
   int traceFlags = 0, result, reportSpace = FALSE;
   State state;      

   while (*(++argv)) {
      if (**argv == '-') {
         while (*(++(*argv))) {
            if (**argv == 't')
               traceFlags |= TRACE_TREE;
            else if (**argv == 'c')
               traceFlags |= TRACE_CODES;
            else if (**argv == 'b')
               traceFlags |= TRACE_BUMPS;
            else if (**argv == 'r')
               traceFlags |= TRACE_RECYCLES;
            else if (**argv == 's')
               reportSpace = TRUE;
         }            
      }
      else {
         StateInit(&state, *argv);         
         LZWCmpInit(&cmp, ProcessCodes, &state, RECYCLE_CODE, traceFlags);   
         
         while ((result = fgetc(state.read)) != EOF)           
            LZWCmpEncode(&cmp, result);         
         
         LZWCmpStop(&cmp);         
       
         if (reportSpace)
            printf("Space after LZWCmpStop for %s: %ld\n",
             *argv, report_space());
        
         StateStop(&state);        
         LZWCmpDestruct(&cmp);         
      }
   }
 
   if (reportSpace)
      printf("Final space: %ld\n", report_space());
   
   return 0;
}

static void ProcessCodes(void *state, UInt code, int end) { 
   State *dataState = state;
   
   if (end) {      
      fputc('\n', dataState->write);
   }     
   else {
      fprintf(dataState->write, "%08X", code);
      dataState->codesOnLine++;
      
      if (dataState->codesOnLine == CODES_PER_LINE) {         
         fputc('\n', dataState->write);        
         dataState->codesOnLine = 0;
      }         
      else
         fputc(' ', dataState->write);     
   }
}

static void StateInit(State *state, char *argument) {
   state->read = fopen(argument, "r");
   state->fileName = malloc(sizeof(char) * (strlen(argument)
    + strlen(".Z") + 1));
    
   state->fileName = strcpy(state->fileName, argument);
   state->fileName = strcat(state->fileName, ".Z");   
   state->write = fopen(state->fileName, "w");   
   state->codesOnLine = 0; 
   state->end = FALSE;
}

static void StateStop(State *state) {
   fclose(state->read);
   fclose(state->write);
   free(state->fileName);
}

