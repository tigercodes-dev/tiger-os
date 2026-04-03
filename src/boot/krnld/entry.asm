bits 16

section .entry

extern __bss_start
extern __end

extern krnld_start
global start

start:
    cli

    mov [boot_drive], dl

    mov ax, ds
    mov ss, ax
    mov sp, 0xFFF0
    mov bp, sp

    ; enable A20 line
    in al, 0x92

    test al, 2
    jnz .after_a20

    or al, 2
    and al, 0xFE
    out 0x92, al

.after_a20:
    lgdt [gdt_descriptor]

    mov eax, cr0
    or al, 1
    mov cr0, eax

    jmp dword 0x08:pmode

; Initial GDT, may change later in kernel
init_gdt:
    ; Null Descriptor
    dq 0

    ; 32 bit code
    dw 0xFFFF
    dw 0
    db 0
    db 0b10011010
    db 0b11001111
    db 0

    ; 32 bit data
    dw 0xFFFF
    dw 0
    db 0
    db 0b10010010
    db 0b11001111
    db 0

    ; 16 bit code
    dw 0xFFFF
    dw 0
    db 0
    db 0b10011010
    db 0b00001111
    db 0

    ; 16 bit data
    dw 0xFFFF
    dw 0
    db 0
    db 0b10010010
    db 0b00001111
    db 0

gdt_descriptor:
    dw gdt_descriptor - init_gdt - 1 ; gdt size
    dd init_gdt ; gdt location

boot_drive: db 0

pmode:
    [bits 32]

    mov ax, 0x10
    mov ds, ax
    mov ss, ax

    ; clear bss
    mov edi, __bss_start
    mov ecx, __end
    sub ecx, edi

    xor al, al
    cld
    rep stosb

    xor edx, edx
    mov dl, [boot_drive]

    push edx
    call krnld_start

    cli
    hlt
