#ifndef _I686_PIC_H_
#define _I686_PIC_H_

#include <stdint.h>

void config_PIC(uint8_t offset_pic1, uint8_t offset_pic2);

void end_of_interrupt_PIC(int irq);

void disable_PIC();

void mask_PIC(int irq);

void unmask_PIC(int irq);

uint16_t get_irr_PIC();

uint16_t get_isr_PIC();

#endif