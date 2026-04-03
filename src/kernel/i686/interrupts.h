#ifndef _I686_INTERRUPTS_H_
#define _I686_INTERRUPTS_H_

#include <stdint.h>

// Reverse Order - stack grows downward
typedef struct {
    uint32_t ds;
    uint32_t edi, esi, ebp, k_esp, ebx, edx, ecx, eax; // Pushed by pusha
    uint32_t interrupt, error;
    uint32_t eip, cs, eflags, esp, ss; // Pushed during interrupt
} __attribute__((packed)) InterruptStack;

typedef void (*InterruptHandler)(InterruptStack* stack);

void initialize_ISR();
void register_handler_ISR(int interrupt, InterruptHandler handler);

#endif