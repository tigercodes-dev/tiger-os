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

#endif