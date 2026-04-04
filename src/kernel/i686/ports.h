#ifndef _I686_PORTS_H_
#define _I686_PORTS_H_

#include <stdint.h>
#include <stdbool.h>

// Outputs a byte to a hardware port
void __attribute__((cdecl)) outb(uint16_t port, uint8_t data);

// Takes a byte of input from a hardware port
uint8_t __attribute__((cdecl)) inb(uint16_t port);

// Outputs a word (16 bits) to a hardware port
void __attribute__((cdecl)) outw(uint16_t port, uint8_t data);

// Takes a word (16 bits) of input from a hardware port
uint16_t __attribute__((cdecl)) inw(uint16_t port);

// Outputs a double word (32 bits) to a hardware port
void __attribute__((cdecl)) outl(uint16_t port, uint8_t data);

// Takes a double word (32 bits) of input from a hardware port
uint32_t __attribute__((cdecl)) inl(uint16_t port);

#endif