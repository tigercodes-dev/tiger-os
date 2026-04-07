; Volume Boot Record - TIGER OS volume

; NASM directives
org 0x7C00
bits 16

; FAT16 BIOS Parameter Block

jmp short main
nop

oem_identifier:         db "TIGER OS" ; 8 Bytes
bytes_per_sector:       dw 512
sectors_per_cluster:    db 8
reserved_sectors:       dw 8
fat_count:              db 2
root_dir_entries:       dw 512
total_sectors:          dw 0 ; sector size will be checked in 32-bit sector count instead
media_descriptor:       db 0xF8 ; fixed hard disk
sectors_per_fat:        dw 64
sectors_per_track:      dw 63
disk_heads:             dw 16
hidden_sectors:         dd 0
large_total_sectors:    dd 131009 ; 64MiB / 512 sectors - 63 sectors (MBR sectors)

; Extended BIOS Parameter Block

drive_num:              db 0x80 ; 0x80 = hard disk
winnt_flags:            db 0 ; Ooly used by Windows NT
signature:              db 0x29 ; must be 0x28 or 0x29
vol_serial_number:      dd 0x44E0C6DC ; doesn't matter for this value, just a random number
vol_label:              db "TIGER OS   " ; 11 Bytes, pad with spaces
filesystem_type:        db "FAT16   " ; 8 Bytes

; Code starts here

main:
    mov [drive_num], dl

    push es

    mov ah, 0x08
    int 0x13
    jc disk_error
    
    pop es

    and cl, 0x3F
    xor ch, ch
    mov [sectors_per_track], cx
    inc dh
    mov [disk_heads], dh

    xor ah, ah
    mov al, [fat_count]
    mul word [sectors_per_fat]
    add ax, [reserved_sectors]
    add ax, PARTITION_START

    push ax

    mov ax, [root_dir_entries]
    shl ax, 5 ; Multiply by 32

    xor dx, dx
    div word [bytes_per_sector]

    test dx, dx
    jz .read_root_dir
    inc ax

.read_root_dir:
    mov [root_dir_size], ax
    mov cl, al

    pop ax

    mov dl, [drive_num]
    mov bx, 0x7E00
    call disk_read

    mov bx, 0
    mov si, 0x7E00

.find_kernel_sys:
    mov di, kernel_sys
    mov cx, 11 ; read 11 bytes 8.3 filename

    push si
    repe cmpsb
    pop si

    je .begin_load_kernel

    add si, 32

    inc bx
    cmp bx, [root_dir_entries]
    jl .find_kernel_sys

    jmp kernel_missing_error

.begin_load_kernel:
    mov di, [si + 26] ; First cluster

    mov ax, [reserved_sectors]
    add ax, PARTITION_START
    mov bx, 0x7E00
    mov cl, [sectors_per_fat]

    call disk_read

    mov bx, KERNEL_SEGMENT
    mov es, bx
    mov bx, KERNEL_OFFSET

.load_kernel_cluster:
    mov ax, di
    sub ax, 2

    mul byte [sectors_per_cluster]

    ; sectors before first file = reserved sectors + (sectors per fat * fat count) + root dir size
    add ax, [reserved_sectors]
    add ax, [root_dir_size]

    push bx
    push ax

    mov ax, [sectors_per_fat]
    mul byte [fat_count]

    pop bx
    add ax, bx
    pop bx

    add ax, PARTITION_START

    mov cl, [sectors_per_cluster]

    call disk_read

    ; increase bx position by bytes per cluster - (bytes per sector) * (sectors per cluster)
    push dx
    mov ax, [bytes_per_sector]
    mul word [sectors_per_cluster]
    pop dx
    add bx, ax

    mov ax, di
    shl ax, 1 ; multiply by 2 (2 byte / 16 bit clusters)

    mov si, 0x7E00
    add si, ax

    mov di, [ds:si]

    ; Check if it's the end of the file
    cmp di, 0xFFF8
    jae .jump_to_kernel

    jmp .load_kernel_cluster

.jump_to_kernel:
    mov dl, [drive_num]

    mov ax, KERNEL_SEGMENT
    mov ds, ax
    mov es, ax

    jmp KERNEL_SEGMENT:KERNEL_OFFSET

disk_error:
    mov si, err_disk
    call puts
    jmp reboot

kernel_missing_error:
    mov si, err_kernel
    call puts
    jmp reboot

reboot:
    mov si, msg_reboot
    call puts

    ; wait for keypress
    mov ah, 0x00
    int 16h

    jmp 0xFFFF:0 ; jump to beginning of BIOS

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

; Convert LBA scheme to CHS scheme
; Params:
;   ax - LBA sector address
; Output:
;   cx (low 6 bits) - sector
;   cx (high 10 bits) - cylinder
;   dh - head
lba_to_chs:
    push ax
    push dx

    xor dx, dx
    div word [sectors_per_track]
    
    mov cx, dx
    inc cx ; sector starts at 1

    xor dx, dx
    div word [disk_heads]

    shl dx, 8 ; shift dl into dh

    mov ch, al
    shl ah, 6
    or cl, ah

    pop ax
    mov dl, al
    pop ax

    ret

; Reads sectors from a disk
; Params:
;   dl - drive number
;   ax - LBA address
;   cl - sector read count
;   es:bx - data output buffer
disk_read:
    pusha
    push cx

    call lba_to_chs

    pop ax
    mov ah, 0x02

    stc
    int 0x13
    jc disk_error

    popa

    ret

%define endl 0x0D, 0x0A

; Message Strings (null terminated)

err_disk: db "Disk error.", 0
err_kernel: db "KRNLD.SYS is missing.", 0
msg_reboot: db endl, "Press any key to reboot...", 0

kernel_sys: db "KRNLD   SYS"

root_dir_size: dw 0

KERNEL_SEGMENT: equ 0x0
KERNEL_OFFSET: equ 0x500

PARTITION_START: equ 63

; Padding and BIOS Boot Signature
times 510-($-$$) db 0
db 0x55, 0xAA
