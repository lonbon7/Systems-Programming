#include <stdio.h>

#define strchr
#define strlen

 void TrStr(char *string, char *code) {
   char *source = code;   

   while (*string) {           
      while (*source) {
         if (*string == *source++)
            *string = *source;                     
         source++;                 
      }
      source = code;
      string++;      
   }
}
 

int main() {
   char str1[20], str2[20], *test = "";

   TrStr(test, "abcd");
   printf("Empty translates to |%s|\n", test);

   while (EOF != scanf("%s %s", str1, str2)) {
      TrStr(str1, "");
      printf("%s -> ", str1);
      TrStr(str1, str2);
      printf("%s\n", str1);
   }
   
   return 0;
}