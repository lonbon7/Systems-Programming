#include <stdio.h>
#include <stdlib.h>

#define NUM_VALS 26
#define NUM_CHARS 11
#define MAX_DIFF 7
#define BITS_PER_CHAR 7
#define LSBS 3

void EmbedWatermark(short *vals, char *msg) {
   short shift = 0;
   char charShift = 0;
   int index, total = 0; 
      
   for (index = 0; total < NUM_CHARS * BITS_PER_CHAR; total++) {        
      vals[index] &= ~(1 << shift);
      vals[index] |= ((*msg & (1 << charShift)) >> charShift) << shift;   
      shift++;
      charShift++;
      
      if (shift == LSBS) {
         shift = 0;        
         index++;
      }
      if (charShift == BITS_PER_CHAR) {
         charShift = 0;
         msg++;
      }
   }    
}
    
void PrintWatermark(short *vals) {
   char message = 0;  
   int index, sizeChar = 0, total = 0, shift = 0;
   
   for (index = 0; total < NUM_CHARS * BITS_PER_CHAR; total++){         
      message |=  ((vals[index] & 1 << shift) >> shift) << sizeChar;
      sizeChar++;
      shift++;     
      
      if (shift == LSBS) {
         shift = 0;
         index++;
      }
      if (sizeChar == BITS_PER_CHAR) {
         printf("%c", message);
         message = 0;
         sizeChar = 0;         
      }
   }
   printf("\n");
}

int main() {
   short *vals = calloc(NUM_VALS, sizeof(short));
   char msg[NUM_CHARS];
   int i, diff;
   short copy[NUM_VALS];
   
   for (i = 0; i < NUM_CHARS; i++)
      msg[i] = getchar();
      
   for (i = 0; i < NUM_VALS; i++) {
      scanf("%hd", &vals[i]);
      copy[i] = vals[i];
   }  
   
   EmbedWatermark(vals, msg);
   for (i = 0; i < NUM_CHARS; i++)
      msg[i] = 0;

   for (i = 0; i < NUM_VALS; i++) {
      diff = vals[i] - copy[i];
      if (diff > MAX_DIFF || diff < -MAX_DIFF)
         printf("Val %d differs too much\n", i);
      copy[i] = vals[i];
   }
   
   PrintWatermark(copy);
}
