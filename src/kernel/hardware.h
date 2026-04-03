#ifndef HARDWARE_H
#define HARDWARE_H

#include <stdint.h>
#include <stdbool.h>

// Outputs a byte to a hardware port
void __attribute__((cdecl)) outbyte(uint16_t port, uint8_t data);

// Takes a byte of input from a hardware port
uint8_t __attribute__((cdecl)) inbyte(uint16_t port);

#endif