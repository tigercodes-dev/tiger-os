; NASM directives
org 0x7C00
bits 16

; FAT16 BIOS Parameter Block

jmp short main
nop

oem_identifier:         db "TIGER OS" ; 8 Bytes
bytes_per_sector:       dw 512
sectors_per_cluster:    db 4
reserved_sectors:       dw 4
fat_count:              db 2
root_dir_entries:       dw 512
total_sectors:          dw 0 ; sector size will be checked in 32-bit sector count instead
media_descriptor:       db 0xF8 ; fixed hard disk
sectors_per_fat:        dw 64
sectors_per_track:      dw 32
disk_heads:             dw 4
hidden_sectors:         dd 0
large_total_sectors:    dd 65536 ; 32MiB disk with 512 byte sectors

; Extended BIOS Parameter Block

drive_num:              db 0x80 ; 0x80 = hard disk
winnt_flags:            db 0 ; Ooly used by Windows NT
signature:              db 0x29 ; must be 0x28 or 0x29
vol_serial_number:      dd 0x44E0C6DC ; doesn't matter for this value, just a random number
vol_label:              db "TIGER OS   " ; 11 Bytes, pad with spaces
filesystem_type:        db "FAT16   " ; 8 Bytes

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
