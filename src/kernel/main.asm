org 0x500
bits 16

start:
    mov si, msg_loading
    call puts

    jmp $

puts:
    push ax
    push bx
    push si

    mov ah, 0x0E
    mov bh, 0

.print_loop:
    lodsb

    test al, al
    jz .done

    int 0x10

    jmp .print_loop

.done:
    pop si
    pop bx
    pop ax

    ret

%define endl 0x0D, 0x0A

msg_loading: db "Loading TigerOS...", endl, 0
