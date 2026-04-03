#include "string.h"
#include <stddef.h>
#include <stdint.h>

const char* strchr(const char* s, char c) {
    if (s == NULL) {
        return NULL;
    }

    for (; *s; s++) {
        if (*s == c) {
            return s;
        }
    }

    return NULL;
}

char* strcpy(char* dest, const char* src) {
    char* org_dest = dest;
    if (dest == NULL) {
        return NULL;
    }
    if (src == NULL) {
        *dest = 0;
        return dest;
    }
    
    for (; *src; src++) {
        *dest++ = *src;
    }

    *dest = 0;
    return org_dest;
}

unsigned int strlen(const char* s) {
    unsigned int len = 0;
    for (; *s; s++) {
        len++;
    }
    
    return len;
}
