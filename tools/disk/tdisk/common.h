#pragma once

#include <stdint.h>
#include <stdbool.h>

#define SECTOR_SIZE 512

#define SECTORS_PER_TRACK 63
#define HEADS 16

enum PartitionActive {
    PARTITION_NOT_ACTIVE = 0x00,
    PARTITION_ACTIVE     = 0x80,
};

// Brief list of types. Not all are currently available.
enum SystemType {
    SYSTEM_TYPE_NONE     = 0x00,
    SYSTEM_TYPE_FAT12    = 0x01,
    SYSTEM_TYPE_FAT16    = 0x04,
    SYSTEM_TYPE_FAT16B   = 0x06,
    SYSTEM_TYPE_FAT32CHS = 0x0B,
};

typedef struct {
    uint8_t head;
    uint16_t sec_cyl; // Bits 0-5: sector, Bits 6-15: cylinder 
} __attribute__((packed)) CHS;

typedef struct {
    uint8_t active;
    CHS start_chs;
    uint8_t system_type;
    CHS end_chs;
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

#define min(a, b) (a) < (b) ? (a) : (b)
#define max(a, b) (a) > (b) ? (a) : (b)
