#ifndef _IRQ_H_
#define _IRQ_H_

#include "interrupts.h"

typedef void (*IRQHandler)(InterruptStack* stack);

void initialize_IRQ();
void register_handler_IRQ(int irq, IRQHandler handler);

#endif