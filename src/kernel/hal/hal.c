#include "hal.h"
#include "../i686/gdt.h"
#include "../i686/idt.h"
#include "../i686/interrupts.h"
#include "../i686/irq.h"
#include "../i686/vga.h"
#include "../i686/ports.h"
#include "../log.h"

void clock(InterruptStack* stack) {
    
}

void keypress(InterruptStack* stack) {
    logf(DEBUG, "Scancode: %x", inb(0x60));
}

// Initialize the Hardware Abstraction Layer
void initialize_HAL() {
    clrscreen_VGA();
    initialize_GDT();
    initialize_IDT();
    initialize_ISR();
    initialize_IRQ();

    register_handler_IRQ(0, clock);
    register_handler_IRQ(1, keypress);

    // Initialize Keyboard
    outb(0x64, 0x20);
    uint8_t cfg = inb(0x60);
    cfg &= ~(1 << 6);
    cfg |= 1;
    outb(0x64, 0x60);
    outb(0x60, cfg);
}
