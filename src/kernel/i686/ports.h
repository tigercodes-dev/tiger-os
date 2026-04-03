#ifndef _I686_PORTS_H_
#define _I686_PORTS_H_

#include <stdint.h>
#include <stdbool.h>

// Outputs a byte to a hardware port
void __attribute__((cdecl)) outbyte(uint16_t port, uint8_t data);

// Takes a byte of input from a hardware port
uint8_t __attribute__((cdecl)) inbyte(uint16_t port);

// Outputs a word (16 bits) to a hardware port
void __attribute__((cdecl)) outword(uint16_t port, uint8_t data);

// Takes a word (16 bits) of input from a hardware port
uint16_t __attribute__((cdecl)) inword(uint16_t port);

// Outputs a double word (32 bits) to a hardware port
void __attribute__((cdecl)) outdword(uint16_t port, uint8_t data);

// Takes a double word (32 bits) of input from a hardware port
uint32_t __attribute__((cdecl)) indword(uint16_t port);

#endif