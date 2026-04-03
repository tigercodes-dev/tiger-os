#include "hal.h"
#include "../i686/gdt.h"
#include "../i686/idt.h"

// Initialize the Hardware Abstraction Layer
void initialize_HAL() {
    initialize_GDT();
    initialize_IDT();
}
