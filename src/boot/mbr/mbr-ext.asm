; Extended MBR code for listing partition

org 0x800
bits 16

list_partition:
    push ax
    push si

    push bx
    mov ah, 0x0E
    mov al, cl
    add al, 0x31
    xor bh, bh
    int 0x10
    pop bx

    cmp byte [bx + 4], 0x00
    jne .next1

    mov si, part_empty
    call puts

.next1:
    cmp byte [bx + 4], 0x01
    jne .next2

    mov si, part_fat12
    call puts

    jmp .end

.next2:
    cmp byte [bx + 4], 0x04
    je .next3
    cmp byte [bx + 4], 0x06
    je .next3
    cmp byte [bx + 4], 0x0E
    je .next3

    jmp .next4

.next3:
    mov si, part_fat16
    call puts

    jmp .end

.next4:
    cmp byte [bx + 4], 0x0B
    je .next5
    cmp byte [bx + 4], 0x0C
    je .next5

    jmp .next6

.next5:
    mov si, part_fat32
    call puts

    jmp .end

.next6:
    cmp byte [bx + 4], 0x07
    jne .next7

    mov si, part_exfat_ntfs
    call puts

    jmp .end

.next7:
    cmp byte [bx + 4], 0x83
    jne .next8

    mov si, part_linux
    call puts

    jmp .end

.next8:

.end:
    pop ax
    pop si

    ret

; Prints a string to the screen
; Params:
;  ds:si - points to string
puts:
    push ax
    push bx
    push si

    mov ah, 0x0E ; teletype mode
    mov bh, 0 ; page 0

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

; Messages
%define endl 0x0D, 0x0A

part_empty: db " Empty", 0
part_fat12: db " FAT12", 0
part_fat16: db " FAT16", 0
part_fat32: db " FAT32", 0
part_exfat_ntfs: db " exFAT / NTFS", 0
part_linux: db " Linux", 0
