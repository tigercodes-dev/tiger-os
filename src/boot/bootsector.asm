; NASM directives
org 0x7C00
bits 16

; Code starts here
main:
    mov si, msg_loading
    call puts

    jmp $ ; Infinite loop

; Prints a null-terminated string to the screen
; Params:
;   ds:si - pointer to string
puts:
    push ax
    push bx
    push si

    mov ah, 0x0E ; Teletype mode
    mov bh, 0 ; Page 0

.print_loop:
    lodsb ; [ds:si] -> al, si++
    
    test al, al
    jz .done ; Null character = done printing

    int 0x10

    jmp .print_loop

.done:
    pop si
    pop bx
    pop ax

    ret

%define endl 0x0D, 0x0A

; Message Strings (null terminated)
msg_loading: db "Booting TigerOS...", endl, 0

; Padding and BIOS Boot Signature
times 510-($-$$) db 0
db 0x55, 0xAA
