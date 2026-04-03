#include "idt.h"
#include "descriptor-table.h"

#define SET_FLAG(x, flag) (x) |= (flag);
#define CLR_FLAG(x, flag) (x) &= ~(flag);

IDTGate idt[256];

IDTDescriptor desc = { sizeof(idt) - 1, idt};

void set_gate_IDT(uint8_t interrupt, void* offset, uint16_t segment, uint8_t attrib) {
    idt[interrupt].offset_low = ((uint32_t)offset) & 0xFFFF;
    idt[interrupt].selector = segment;
    idt[interrupt].reserved = 0;
    idt[interrupt].attributes = attrib;
    idt[interrupt].offset_high = (((uint32_t)offset) >> 16) & 0xFFFF;
}

void enable_gate_IDT(uint8_t interrupt) {
    SET_FLAG(idt[interrupt].attributes, ATTRIB_PRESENT);
}

void disable_gate_IDT(uint8_t interrupt) {
    CLR_FLAG(idt[interrupt].attributes, ATTRIB_PRESENT);
}

void initialize_IDT() {
    load_idt(&desc);
}
