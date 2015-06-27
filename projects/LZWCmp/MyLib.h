#ifndef MYLIB_H
#define MYLIB_H

#define BITS_PER_BYTE 8
#define TRUE 1
#define FALSE 0
#define INIT_NUM_BITS 9
#define INIT_MAX_CODE 511
#define RECYCLE_CODE 4096
#define CODES_PER_LINE 8
#define EOD_CODE 256
#define FOUND_CODE 256
#define BITS_PER_INT 32

typedef struct State {
   char *fileName;
   int codesOnLine;
   FILE *write;
   FILE *read;
   int end;
} State;

typedef unsigned char UChar;
typedef unsigned long ULong;
typedef unsigned int UInt;
typedef unsigned short UShort;

#ifdef LITTLE_ENDIAN

#define SwapULong(val) (val << 24 | (val << 8 & 0xFF0000) |\
 (val >> 8 & 0xFF00) | val >> 24 & 0xFF)
#define SwapUShort(val) (val << BITS_PER_BYTE | val >> BITS_PER_BYTE)

#else

#define SwapULong(val) (val)
#define SwapUShort(val) (val)

#endif

#endif

