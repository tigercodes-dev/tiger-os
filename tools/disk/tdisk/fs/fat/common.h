#pragma once

#include "../../common.h"

typedef struct {
    uint8_t jmp_instruction[3];
    uint8_t oem_identifier[8];
    uint16_t bytes_per_sector;
    uint8_t sectors_per_cluster;
    uint16_t reserved_sectors;
    uint8_t fat_count;
    uint16_t root_dir_entries;
    uint16_t total_sectors;
    uint8_t media_descriptor;
    uint16_t sectors_per_fat;
    uint16_t sectors_per_track;
    uint16_t heads;
    uint32_t hidden_sectors;
    uint32_t large_sector_count;
    
    uint8_t drive_num;
    uint8_t __reserved;
    uint8_t signature;
    uint32_t serial_num;
    uint8_t volume_label[11];
    uint8_t system_id[8];
} __attribute__((packed)) FAT12_16BootRecord;

typedef union {
    VBR vbr_bytes;
    FAT12_16BootRecord fat_boot_record;
} FAT12_16VBR;

