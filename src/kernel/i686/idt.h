#ifndef _I686_IDT_
#define _I686_IDT_

#include <stdint.h>

typedef struct {
    uint16_t offset_low;
    uint16_t selector;
    uint8_t reserved;
    uint8_t attributes;
    uint16_t offset_high;
} __attribute__((packed)) IDTGate;

typedef struct {
    uint16_t limit;
    IDTGate* entries;
} __attribute__((packed)) IDTDescriptor;

enum IDTAttributes {
    ATTRIB_TYPE_TASK       = 0x05, // Task Gate
    ATTRIB_TYPE_16BIT_INT  = 0x06, // 16-bit Interrupt Gate
    ATTRIB_TYPE_16BIT_TRAP = 0x07, // 16-bit Trap Gate
    ATTRIB_TYPE_32BIT_INT  = 0x0E, // 32-bit Interrupt Gate
    ATTRIB_TYPE_32BIT_TRAP = 0x0F, // 32-bit Trap Gate

    ATTRIB_RING_0          = 0 << 5,
    ATTRIB_RING_1          = 1 << 5,
    ATTRIB_RING_2          = 2 << 5,
    ATTRIB_RING_3          = 3 << 5,

    ATTRIB_PRESENT         = 0x80, // Must be set for the gate to be valid
};

// Load the Interrupt Descriptor Table
void initialize_IDT();
// Sets an interrupt gate for an interrupt number
void set_gate_IDT(uint8_t interrupt, void* offset, uint16_t segment, uint8_t attrib);
// Enable an interrupt gate for an interrupt number
void enable_gate_IDT(uint8_t interrupt);
// Disable the interrupt gate for an interrupt number
void disable_gate_IDT(uint8_t interrupt);

#endif