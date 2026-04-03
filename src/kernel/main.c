#include <stdint.h>
#include "memory.h"
#include "stdio.h"
#include "hal/hal.h"

extern uint8_t __entry_start;
extern uint8_t __bss_start;
extern uint8_t __end;

void __attribute__((section(".entry"))) start(uint16_t boot_drive) {
    memset(&__bss_start, 0, (&__end) - (&__bss_start)); // Clear bss data

    clrscreen();
    printf("/SYSTEM/KERNEL.SYS loaded at 0x%lx\n", &__entry_start);

    puts("Initializing the Hardware Abstraction Layer...\n");
    
    initialize_HAL();

    for (;;);
}
