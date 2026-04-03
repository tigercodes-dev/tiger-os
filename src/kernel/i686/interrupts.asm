bits 32

%macro ISR 1

global ISR%1
ISR%1:
    push 0
    push %1
    jmp ISR_handler

%endmacro

%macro ISR_ERRCODE 1

global ISR%1
ISR%1:
    push %1
    jmp ISR_handler

%endmacro

%include "i686/isr_gen/isr_gen.inc"

; Common handler for all interrupts

extern handle_interrupt

ISR_handler:
    pusha

    mov eax, 0
    mov ax, ds

    push eax
    mov ax, 0x10
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax

    push esp
    call handle_interrupt ; C Function
    add esp, 4

    pop eax
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax

    popa
    add esp, 8

    iret ; special return from interrupt
