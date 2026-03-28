#ifndef KLD_HARDWARE_H
#define KLD_HARDWARE_H

#include <stdint.h>
#include <stdbool.h>

// Outputs a byte to a hardware port
void __attribute__((cdecl)) outbyte(uint16_t port, uint8_t data);

// Takes a byte of input from a hardware port
uint8_t __attribute__((cdecl)) inbyte(uint16_t port);

// Gets the drive parameters of a drive. Outputs true/false if the operation was sucessful or not.
bool __attribute__((cdecl)) disk_getparams(uint8_t drive, uint8_t* drive_type_out, uint16_t* cyls_out, uint16_t* secs_out, uint16_t* heads_out);

// Resets a disk controller.
bool __attribute__((cdecl)) disk_reset(uint8_t drive);

// Reads sectors from a disk
bool __attribute__((cdecl)) disk_read(uint8_t drive, uint16_t cylinder, uint16_t sector, uint16_t head, uint8_t count, void* data_out);

#endif