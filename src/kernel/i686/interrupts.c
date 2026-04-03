#include "interrupts.h"
#include "../stdio.h"
#include "../shutdown.h"
#include "idt.h"
#include <stddef.h>

InterruptHandler handlers[256];

static const char* const exceptions[] = {
    "Division Error",
    "Debug",
    "Non-maskable Interrupt",
    "Breakpoint",
    "Overflow",
    "Bound Range Exceeded",
    "Invalid Opcode",
    "Device Not Available",
    "Double Fault",
    "Coprocessor Segment Overrun",
    "Invalid TSS",
    "Segment Not Present",
    "Stack-Segment Fault",
    "General Protection Fault",
    "Page Fault",
    "",
    "x87 Floating Point Exception",
    "Alignment Check",
    "Machine Check",
    "SIMD Floating-Point Exception",
    "Virtualization Exception",
    "Control Protection Exception",
    "",
    "",
    "",
    "",
    "",
    "",
    "Hypervisor Injection Exception",
    "VMM Communication Exception",
    "Security Exception",
    ""
};

void initialize_gates_IDT();

void initialize_ISR() {
    initialize_gates_IDT();
    for (int i = 0; i < 256; i++) {
        enable_gate_IDT(i);
    }
}

void register_handler_ISR(int interrupt, InterruptHandler handler) {
    handlers[interrupt] = handler;
    enable_gate_IDT(interrupt);
}

void __attribute__((cdecl)) handle_interrupt(InterruptStack* stack) {
    if (handlers[stack->interrupt] != NULL) {
        handlers[stack->interrupt](stack);
    } else if (stack->interrupt >= 32) {
        printf("Interrupt 0x%x\n", stack->interrupt);
    } else {
        printf("Exception 0x%x occured: %s\n", stack->interrupt, exceptions[stack->interrupt]);

        puts("Dumping CPU State:\n");
        printf("  eax=0x%x ebx=0x%x ecx=0x%x edx=0x%x esi=0x%x edi=0x%x\n", stack->eax, stack->ebx, stack->ecx, stack->edx, stack->esi, stack->edi);
        printf("  esp=0x%x ebp=0x%x eip=0x%x eflags=0x%x cs=0x%x ds=0x%x ss=0x%x\n", stack->esp, stack->ebp, stack->eip, stack->eflags, stack->cs, stack->ds, stack->ss);
        printf("  interrupt=0x%x error=0x%x\n", stack->interrupt, stack->error);

        puts("Fatal Error. System cannot continue.\n");
        halt_system();
    }
}
