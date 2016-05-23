#ifndef STDIO_H
#define STDIO_H

#include <stdio.h>
#include <stdarg.h>

void PrintChar(char c);
signed int PutChar(char *pStr, char c);
signed int PutString(char *pStr, const char *pSource);
signed int PutUnsignedInt(
    char *pStr,
    char fill,
    signed int width,
    unsigned int value);
signed int PutSignedInt(
    char *pStr,
    char fill,
    signed int width,
    signed int value);
signed int PutHexa(
    char *pStr,
    char fill,
    signed int width,
    unsigned char maj,
    unsigned int value);
signed int vsnprintf(char *pStr, size_t length, const char *pFormat, va_list ap);
signed int snprintf(char *pString, size_t length, const char *pFormat, ...);
signed int vsprintf(char *pString, const char *pFormat, va_list ap);
signed int vfprintf(FILE *pStream, const char *pFormat, va_list ap);
signed int vprintf(const char *pFormat, va_list ap);
signed int fprintf(FILE *pStream, const char *pFormat, ...);
signed int printf(const char *pFormat, ...);
signed int sprintf(char *pStr, const char *pFormat, ...);
signed int puts(const char *pStr);
signed int fputc(signed int c, FILE *pStream);
signed int fputs(const char *pStr, FILE *pStream);


#endif
