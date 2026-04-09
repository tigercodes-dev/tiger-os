#pragma once

#include <stdint.h>
#include <stdbool.h>

#define SECTOR_SIZE 512

enum PartitionActive {
    PARTITION_NOT_ACTIVE = 0x00,
    PARTITION_ACTIVE     = 0x80,
};

// Brief list of types. Not all are currently available.
enum PartitionTypes {
    PARTITION_TYPE_NONE     = 0x00,
    PARTITION_TYPE_FAT12    = 0x01,
    PARTITION_TYPE_FAT16    = 0x04,
    PARTITION_TYPE_FAT16B   = 0x06,
    PARTITION_TYPE_FAT32CHS = 0x0B,
};

typedef struct {
    uint8_t active;
    uint8_t start_head;
    uint16_t start_sec_cyl; // Bits 0-5: sector, Bits 6-15: cylinder 
    uint8_t partition_type;
    uint8_t end_head;
    uint16_t end_sec_cyl; // Bits 0-5: sector, Bits 6-15: cylinder 
    uint32_t start_lba;
    uint32_t partition_sectors;
} __attribute__((packed)) PartitionTableEntry;

typedef struct {
    uint8_t bootstrap_code[440];
    uint32_t disk_signature;
    uint16_t reserved;
    PartitionTableEntry partition_table[4];
    uint16_t boot_signature;
} __attribute__((packed)) MBR;
