#ifndef MEMORY_H
#define MEMORY_H

#include <stdint.h>

// Copies bytes from src to dest
void* __attribute__((cdecl)) memcpy(void* dest, const void* src, int n);

// Sets bytes to a value
void* __attribute__((cdecl)) memset(void* ptr, uint8_t value, int n);

// Compares bytes of memory
int __attribute__((cdecl)) memcmp(const void* ptr1, const void* ptr2, int n); 

#endif