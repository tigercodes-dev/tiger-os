; Master Boot Record
org 0x600
bits 16

start:
    xor ax, ax
    mov ds, ax
    mov es, ax
    mov ss, ax

    mov sp, 0x600

    ; Move the MBR code to 0x0000:0x0600
    mov si, 0x7C00
    mov di, 0x600
    mov cx, 512
    cld
    rep movsb

    jmp 0x0000:main

main:
    mov si, msg_welcome
    call puts

    mov ah, 0x02
    mov dh, 0
    mov cx, 2
    mov al, 1
    mov bx, 0x800
    int 0x13

    xor cx, cx ; cl - loop counter, ch - partitions found count
    mov bx, partition_table

.check_partition:
    ; Search partition table
    mov al, [bx]
    test al, 0x80 ; test for any active
    jz .partition_listed ; skip listing non active partitions

    call 0x800

    push bx
    mov bl, 1
    shl bl, cl
    or [active_partitions], bl
    pop bx

    inc ch
    mov [last_partition], cl

    mov si, newline
    call puts

.partition_listed:
    inc cl
    cmp cl, 4
    jge .found_all_partitions

    add bx, 16 ; each partition entry is 16 bytes

    jmp .check_partition

.found_all_partitions:
    mov al, [active_partitions]
    test al, al
    jz no_partitions
    
    push cx ; save ch for checking later
    dec ch
    jnz .select_partition
    
    mov cl, [last_partition]
    jmp .boot_partition

.select_partition:
    mov si, msg_select_partition
    call puts

    mov ah, 0x00
    int 0x16 ; Get user input
    mov ah, 0x0E
    xor bh, bh
    int 0x10

    mov si, newline
    call puts

    mov cl, al
    sub cl, 0x31
    cmp cl, 4
    jge .invalid_partition

    mov al, 1
    shl al, cl
    test [active_partitions], al
    jz .invalid_partition

.boot_partition:
    mov al, 16
    mul cl
    add ax, partition_table
    mov bp, ax

    mov ah, 0x02 ; read from disk
    mov al, 1 ; read one sector
    mov dh, [bp + 1] ; head of first sector of partition
    mov cx, [bp + 2]
    mov bx, 0x7C00
    stc

    int 0x13
    jc disk_error

    cmp word [0x7DFE], 0xAA55
    jne .non_bootable

    mov si, newline
    call puts

    ; Reset register values
    xor ax, ax
    xor bx, bx
    xor cx, cx
    xor dh, dh
    xor si, si
    xor di, di
    xor bp, bp
    mov sp, 0x7C00
    mov ds, ax
    mov es, ax
    mov ss, ax

    jmp 0x0000:0x7C00

.non_bootable:
    mov si, msg_non_bootable
    call puts

    pop cx
    dec ch
    jz reboot

    jmp .found_all_partitions

.invalid_partition:
    mov si, msg_invalid
    call puts
    jmp .found_all_partitions

disk_error:
    mov si, msg_disk_error
    call puts
    jmp reboot

no_partitions:
    mov si, msg_no_partitions
    call puts
    jmp reboot

reboot:
    mov ah, 0x00
    int 0x16

    jmp 0xFFFF:0

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

last_partition: db 0
active_partitions: db 0 ; bit 0 - partition 1, bit 1 - partition 2, etc.

; Messages
%define endl 0x0D, 0x0A

msg_welcome: db endl, "Finding partitions...", endl, endl, 0
msg_select_partition: db endl, "Select a partition: ", 0
msg_invalid: db "Invalid partition.", endl, 0
msg_disk_error: db "Disk error.", 0
msg_no_partitions: db "No partitions found.", 0
msg_non_bootable: db "Partition not bootable.", endl, 0

newline: db endl, 0

times 440-($-$$) db 0

; Disk Signature
disk_signature: dd 0x9A2C7E08 ; just a random number
dw 0x0000

; Partition Table
; Entries are added by TDISK
partition_table:

dq 0, 0
dq 0, 0
dq 0, 0
dq 0, 0

; BIOS Boot Signature - End of MBR
db 0x55, 0xAA
