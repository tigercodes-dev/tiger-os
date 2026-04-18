#include "hal.h"
#include "../i686/gdt.h"
#include "../i686/idt.h"
#include "../i686/interrupts.h"
#include "../i686/irq.h"
#include "../i686/vga.h"
#include "../i686/keyboard.h"

void clock(InterruptStack* stack) {
    
}

// Initialize the Hardware Abstraction Layer
void initialize_HAL() {
    clrscreen_VGA();
    initialize_GDT();
    initialize_IDT();
    initialize_ISR();
    initialize_IRQ();

    register_handler_IRQ(0, clock);

    initialize_keyboard();
    set_typing_enabled_keyboard(true);
}
