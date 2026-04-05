#include "hal.h"
#include "../i686/gdt.h"
#include "../i686/idt.h"
#include "../i686/interrupts.h"
#include "../i686/irq.h"

// Initialize the Hardware Abstraction Layer
void initialize_HAL() {
    initialize_GDT();
    initialize_IDT();
    initialize_ISR();
    initialize_IRQ();
}
