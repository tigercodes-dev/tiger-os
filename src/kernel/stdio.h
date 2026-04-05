#ifndef _STDIO_H_
#define _STDIO_H_

#include "hal/vfs.h"
#include <stdarg.h>

// Prints a character to STDOUT
void putc(char c);

// Prints a string to STDOUT
void puts(const char* s);

// Prints an unsigned number with a specified base (2-16) to STDOUT
void print_uint(unsigned long long number, int base);

// Prints a signed number with a specified base (2-16) to STDOUT
void print_int(signed long long number, int base);

// Prints a formatted string to STDOUT
void printf(const char* fmt, ...);

// Prints a character to a file
void fputc(char c, fd_t file);

// Prints a string to a file
void fputs(const char* s, fd_t file);

// Prints an unsigned number with a specified base (2-16) to a file
void fprint_uint(fd_t file, unsigned long long number, int base);

// Prints a signed number with a specified base (2-16) to a file
void fprint_int(fd_t file, signed long long number, int base);

// Prints a formatted string to a file
void fprintf(fd_t file, const char* fmt, ...);

// Prints a formatted string to a file using a va_list
void vfprintf(fd_t file, const char* fmt, va_list args);

#endif