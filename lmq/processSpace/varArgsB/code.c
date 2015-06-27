#include <stdio.h>
#include <stdarg.h>

double addEm(char *format, ...) {   
   va_list params;
   va_start(params, format);
   double total = 0;
   
   for (; *format; format++) {
      if (*format == 'd')
         total +=  va_arg(params, double);
      else if (*format == 'f')
         total += va_arg(params, double);
      else if (*format == 'i')
         total += va_arg(params, int);
      else if (*format == 's')
         total += va_arg(params, int);
      else if (*format == 'c')
         total += va_arg(params, int);
   }
   
   return total;
}

int main() {
   double dVal;
   float fVal;
   int iVal;
   short sVal;
   char cVal;
  
   scanf("%d %hd %c %f %lf", &iVal, &sVal, &cVal, &fVal, &dVal);
  
   printf("Total is %0.2f or %0.2f if you add 42\n",
    addEm("fdisc", fVal, dVal, iVal, sVal, cVal),
    addEm("siccddf", sVal, iVal, cVal, 42, dVal, 0.0, fVal));
}