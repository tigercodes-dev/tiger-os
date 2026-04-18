#ifndef KLD_CTYPE_H
#define KLD_CTYPE_H

#include <stdbool.h>

// Returns if a character is lowercase
static inline bool islower(char c) {
    return c >= 'a' && c <= 'z';
}

// Returns an uppercase version of the character if it is lowercase
static inline char toupper(char c) {
    return islower(c) ? (c - 32) : c;
}

// Returns if a character is uppercase
static inline bool isupper(char c) {
    return c >= 'A' && c <= 'Z';
}

// Returns an lowercase version of the character if it is uppercase
static inline char tolower(char c) {
    return isupper(c) ? (c + 32) : c;
}

// Returns if a character is printable.
static inline bool isprint(unsigned char c) {
    return c >= 0x20 && c < 0x7F;
}

#endif