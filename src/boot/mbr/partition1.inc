; TigerOS MBR Partition Table Entry #1

; Partition 1
part1_status: db 0x80 ; bit 7 set - disk is active
; CHS address of first sector
part1_head: db 1
part1_cyl_sec: dw (1 | (0 << 6)) ; bits 0-5 -> sector   bits 6-15 -> cylinder

part1_type: db 0x06 ; FAT16 with more than 65535 sectors

; CHS address of last sector
part1_head_last: db 0
part1_cyl_sec_last: dw (32 | (130 << 6))

first_sector_lba: dd 63
total_sectors: dd 131009