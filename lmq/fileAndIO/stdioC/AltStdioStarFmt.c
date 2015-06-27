#include <fcntl.h>
#include <unistd.h>
#include <stdarg.h>
#include "AltStdio.h"

#define printf ERROR
#define fprintf ERROR

ALT_FILE altFiles[NUM_FILES] = {
   {0, altFiles[0].buffer, FILE_READ, 0}, {1, altFiles[1].buffer, 0, BUF_SIZE},
   {2, altFiles[2].buffer, 0, 1}, {-1}, {-1}, {-1}, {-1}, {-1},
   {-1}, {-1}
};

ALT_FILE *altStdin = altFiles+0;
ALT_FILE *altStdout = altFiles+1;
ALT_FILE *altStderr = altFiles+2;

int altPutc(char c, ALT_FILE *fp) {
   *fp->next++ = c;
   if (fp->next == fp->buffer + fp->bufSize)
      altFflush(fp);
}

int altFflush(ALT_FILE *fp) {
   int res;
   
   if (fp->fd < 0 || fp->flags & FILE_ATEOF)
      return EOF;

   if (fp->flags & FILE_READ) {
      res = read(fp->fd, fp->buffer, BUF_SIZE);
      if (res == 0)
         fp->flags |= FILE_ATEOF;
      fp->bufSize = res;
   }
   else
      res = write(fp->fd, fp->buffer, fp->next - fp->buffer);
	  
   fp->next = fp->buffer;
   return res < 0 ? EOF : 0;
}
#define getVAInt(p) va_arg(p, int)
#define getVAStr(p) va_arg(p, char *)

int altFprintf(ALT_FILE *fp, char *format, ...) {
   va_list params;
   va_start(params, format);
   char *string, iVal[NUM_FILES], *temp;
   unsigned int ndx, value;
   int fieldSize, found = 0, star, size, index;
   
   while (*format) {
      if (*format == '%') {        
         size = 0;
         star = 0;        
          if (*++format == '*') {
            format++;
            fieldSize = getVAInt(params);
            star = 1;
         }
         
         switch (*format) {        
         case 'c':
            if (fieldSize > 1 && star) {
               ndx = fieldSize - 1;
               while (ndx--)
                  altPutc(' ', fp);               
            }
               
            altPutc(getVAInt(params), fp);
            break;
         case 'i':
            value = getVAInt(params);              
            
            ndx = NUM_FILES - 1;
            
            do {
               iVal[ndx--] = '0' + value % NUM_FILES;
               value /= NUM_FILES;
               size++;
            } while (value > 0);
            
            if (fieldSize > size && star) { 
               index = fieldSize - size; 
               while (index--)
                  altPutc(' ', fp);               
            }           
            
            while (ndx < NUM_FILES - 1)
               altPutc(iVal[++ndx], fp);
            break;
         case 's':           
            string = getVAStr(params);
            
            temp = string;
            for (; *temp++; size++)
               ;
                              
            if (fieldSize > size && star) { 
               ndx = fieldSize - size;
               while(ndx--)
                  altPutc(' ', fp);               
            }            
            
            while (*string)
               altPutc(*string++, fp);
            break;
         default:
            altPutc('%', fp);
            altPutc(*format, fp);
         }         
      }
      else
         altPutc(*format, fp);
      format++;
   }
   return 1;  // No one checks this anyway.
}
