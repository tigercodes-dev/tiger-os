; Volume Boot Record - TIGER OS volume

; NASM directives
org 0x7C00
bits 16

; FAT32 BIOS Parameter Block

jmp short main
nop

oem_identifier:         db "TIGER OS" ; 8 Bytes
bytes_per_sector:       dw 512
sectors_per_cluster:    db 1
reserved_sectors:       dw 32
fat_count:              db 2
root_dir_entries:       dw 0 ; only used by FAT12/16 - FAT32 root dir is in data section
total_sectors:          dw 0 ; sector size will be checked in 32-bit sector count instead
media_descriptor:       db 0xF8 ; fixed hard disk
sectors_per_fat_bad:    dw 0 ; Not used by FAT32
sectors_per_track:      dw 63
disk_heads:             dw 12
hidden_sectors:         dd 63
large_total_sectors:    dd 131009 ; 64MiB / 512 sectors - 63 sectors (MBR sectors)

; Extended BIOS Parameter Block

sectors_per_fat:        dd 1008
ebr_flags:              dw 0
fat_version:            dw 0
root_dir_cluster:       dd 2
fs_info_sector:         dw 1
backup_bs_sector:       dw 6
                        times 12 db 0 ; reserved bytes
drive_num:              db 0x80 ; 0x80 = hard disk
winnt_flags:            db 0 ; reserved
signature:              db 0x29 ; must be 0x28 or 0x29
vol_serial_number:      dd 0x44E0C6DC ; doesn't matter for this value, just a random number
vol_label:              db "TIGER OS   " ; 11 Bytes, pad with spaces
filesystem_type:        db "FAT32   " ; 8 Bytes

; Code starts here

main:
    mov si, msg_load
    call puts

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

    mov ax, [reserved_sectors]
    mov cl, 64
    mov bx, 0x7E00
    mov dl, [drive_num]
    call disk_read

    mov di, [krnld_cluster]
    mov bx, KRNLD_SEGMENT
    mov es, bx
    mov bx, KRNLD_OFFSET

.load_krnld:
    push dx
    mov ax, [sectors_per_fat]
    xor cx, cx
    mov cl, [fat_count]
    mul cx
    add ax, [reserved_sectors]
    pop dx

    push ax
    push dx
    mov ax, di
    sub ax, 2
    xor cx, cx
    mov cl, [sectors_per_cluster]
    mul cx
    pop dx
    pop cx

    add ax, cx

    mov cl, [sectors_per_cluster]
    mov dl, [drive_num]
    call disk_read

    push dx

    mov ax, [bytes_per_sector]
    xor dx, dx
    mov dl, [sectors_per_cluster]
    mul dx

    pop dx

    add bx, ax

    mov bp, di
    shl bp, 2 ; multiply by 2
    
    mov eax, [bp + 0x7E00]
    cmp eax, 0x0FFFFFF8
    jge .jump_to_krnld

    mov di, ax
    jmp .load_krnld

.jump_to_krnld:
    mov dl, [drive_num]

    mov ax, KRNLD_SEGMENT
    mov ds, ax
    mov es, ax

    jmp KRNLD_SEGMENT:KRNLD_OFFSET

disk_error:
    mov si, err_disk
    call puts
    jmp reboot

reboot:
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

    add ax, [hidden_sectors]
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

msg_load: db "TigerOS loading...", 0
err_disk: db "Disk error.", 0

data_section_start: dw 0

krnld_cluster: dw 3

KRNLD_SEGMENT: equ 0x0
KRNLD_OFFSET: equ 0x500

; Padding and BIOS Boot Signature
times 510-($-$$) db 0
db 0x55, 0xAA
