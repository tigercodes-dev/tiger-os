#include "irq.h"
#include "pic.h"
#include <stddef.h>
#include "../stdio.h"
#include "ports.h"

#define PIC_OFFSET 0x20

static IRQHandler handlers[16];

void handle_IRQ(InterruptStack* stack) {
    int irq = stack->interrupt - PIC_OFFSET;
    uint8_t pic_isr = get_isr_PIC();
    uint8_t pic_irr = get_irr_PIC();

    if (handlers[irq] == NULL) {
        printf("Unhandled Hardware IRQ %d\n", irq);
        printf("ISR=0x%x  IRR=0x%x\n", pic_isr, pic_irr);
    } else {
        handlers[irq](stack);
    }

    // Send end of interrupt message
    end_of_interrupt_PIC(irq);
}

void initialize_IRQ() {
    config_PIC(PIC_OFFSET, PIC_OFFSET + 8);
    for (int i = 0; i < 16; i++) {
        register_handler_ISR(PIC_OFFSET + i, handle_IRQ);
    }

    enable_interrupts();
}

void register_handler_IRQ(int irq, IRQHandler handler) {
    handlers[irq] = handler;
}
