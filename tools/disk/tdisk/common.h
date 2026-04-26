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
    SYSTEM_TYPE_XNXRT    = 0x02,
    SYSTEM_TYPE_XNXUSR   = 0x03,
    SYSTEM_TYPE_FAT16    = 0x04,
    SYSTEM_TYPE_EXTD     = 0x05,
    SYSTEM_TYPE_FAT16B   = 0x06,
    SYSTEM_TYPE_HPNTEXF  = 0x07,
    SYSTEM_TYPE_AIX      = 0x08,
    SYSTEM_TYPE_AIXBT    = 0x09,
    SYSTEM_TYPE_OS2BTMGR = 0x0A,
    SYSTEM_TYPE_FAT32CHS = 0x0B,
    SYSTEM_TYPE_FAT32LBA = 0x0C,
    SYSTEM_TYPE_FAT16LBA = 0x0E,
    SYSTEM_TYPE_EXTDLBA  = 0x0F,
    SYSTEM_TYPE_H_FAT12  = 0x11,
    SYSTEM_TYPE_CFGSVC   = 0x12,
    SYSTEM_TYPE_H_FAT16  = 0x14,
    SYSTEM_TYPE_H_EXTD   = 0x15,
    SYSTEM_TYPE_H_FAT16B = 0x16,
    SYSTEM_TYPE_H_HPNTEX = 0x17,
    SYSTEM_TYPE_H_F32CHS = 0x1B,
    SYSTEM_TYPE_H_F32LBA = 0x1C,
    SYSTEM_TYPE_H_F16LBA = 0x1E,
    SYSTEM_TYPE_H_EXTLBA = 0x1F,
    SYSTEM_TYPE_NECDOS   = 0x24,
    SYSTEM_TYPE_WINRE    = 0x27,
    SYSTEM_TYPE_AIXJFS   = 0x35,
    SYSTEM_TYPE_PLAN9    = 0x39,
    SYSTEM_TYPE_PMREC    = 0x3C,
    SYSTEM_TYPE_PREPBOOT = 0x41,
    SYSTEM_TYPE_SFS      = 0x42,
    SYSTEM_TYPE_QNX4_1   = 0x4D,
    SYSTEM_TYPE_QNX4_2   = 0x4E,
    SYSTEM_TYPE_QNX4_3   = 0x4F,
    SYSTEM_TYPE_GNUHURD  = 0x63,
    SYSTEM_TYPE_MINIXOLD = 0x80,
    SYSTEM_TYPE_MINIX    = 0x81,
    SYSTEM_TYPE_LSWP_SLS = 0x82,
    SYSTEM_TYPE_LINUX    = 0x83,
    SYSTEM_TYPE_LINEXTD  = 0x85,
    SYSTEM_TYPE_LINPLTXT = 0x88,
    SYSTEM_TYPE_LINLVM   = 0x8E,
    SYSTEM_TYPE_AMOEBA   = 0x93,
    SYSTEM_TYPE_BSDOS    = 0x9F,
    SYSTEM_TYPE_FREEBSD  = 0xA5,
    SYSTEM_TYPE_OPENBSD  = 0xA6,
    SYSTEM_TYPE_DWNUFS   = 0xA8,
    SYSTEM_TYPE_DWNBOOT  = 0xAB,
    SYSTEM_TYPE_HFS      = 0xAF,
    SYSTEM_TYPE_SLS_BOOT = 0xBE,
    SYSTEM_TYPE_SOLARIS  = 0xBF,
    SYSTEM_TYPE_DELLUTIL = 0xDE,
    SYSTEM_TYPE_GPT      = 0xEE,
    SYSTEM_TYPE_EFI      = 0xEF,
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

typedef struct {
    uint8_t boot_code[510];
    uint16_t boot_signature;
} __attribute__((packed)) VBR;

extern uint8_t const ZERO_BLOCK[SECTOR_SIZE];
