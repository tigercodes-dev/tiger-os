#include <stdint.h>
#include "memory.h"
#include "stdio.h"
#include "hal/hal.h"
#include "i686/shutdown.h"
#include "i686/irq.h"
#include "i686/ports.h"

extern uint8_t __entry_start;
extern uint8_t __bss_start;
extern uint8_t __end;

void clock(InterruptStack* stack) {
    putc('.');
}

void keypress(InterruptStack* stack) {
    printf("\nScancode: %x\n", inb(0x60));
}

void __attribute__((section(".entry"))) start(uint16_t boot_drive) {
    memset(&__bss_start, 0, (&__end) - (&__bss_start)); // Clear bss data

    clrscreen();
    debug_printf("Kernel loaded at 0x%lx\n", &__entry_start);

    puts("Starting Kernel...\n");
    puts("Initializing the Hardware Abstraction Layer...\n");
    
    initialize_HAL();

    register_handler_IRQ(0, clock);
    register_handler_IRQ(1, keypress);

    outb(0x64, 0x20);
    uint8_t cfg = inb(0x60);
    cfg &= ~(1 << 6);
    cfg |= 1;
    outb(0x64, 0x60);
    outb(0x60, cfg);

    for (;;);
}
