#ifndef KLD_STDIO_H
#define KLD_STDIO_H

// Clears the text on the screen
void clrscreen();

// Prints a character to the string
void putc(char c);

// Prints a string to the string
void puts(const char* s);

// Prints an unsigned number with a specified base (2-16)
void print_uint(unsigned long long number, int base);

// Prints a signed number with a specified base (2-16)
void print_int(signed long long number, int base);

// Prints a formatted string to the screen
void printf(const char* fmt, ...);

#endif