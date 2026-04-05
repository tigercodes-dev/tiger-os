#ifndef _STDIO_H_
#define _STDIO_H_

// Clears the text on the screen
void clrscreen();

// Prints a character to the screen
void putc(char c);

// Prints a string to the screen
void puts(const char* s);

// Prints an unsigned number with a specified base (2-16)
void print_uint(unsigned long long number, int base);

// Prints a signed number with a specified base (2-16)
void print_int(signed long long number, int base);

// Prints a formatted string to the screen
void printf(const char* fmt, ...);

// Prints a character to the debug output
void debug_putc(char c);

// Prints a string to the debug output
void debug_puts(const char* s);

// Prints a formatted string to the debug output
void debug_printf(const char* fmt, ...);

#endif