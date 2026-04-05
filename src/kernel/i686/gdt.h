#ifndef _I686_GDT_
#define _I686_GDT_

#include <stdint.h>

typedef struct {
    uint16_t limit_low;
    uint16_t base_low;
    uint8_t base_mid;
    uint8_t access;
    uint8_t limit_high_flags;
    uint8_t base_high;
} __attribute__((packed)) GDTSegment;

typedef struct {
    uint16_t limit;
    GDTSegment* entries;
} __attribute__((packed)) GDTDescriptor;

enum GDTAccess {
    ACCESS_CS_READABLE   = 0x02, // The code segment is readable
    ACCESS_DS_WRITABLE   = 0x02, // The data segment is writable

    ACCESS_CS_CONFORMING = 0x04, // The code segment can execute code of equal or lower privilege level
    ACCESS_DS_DOWNWARD   = 0x04, // The data segment grows downward instead of upward

    ACCESS_CODE_SEG      = 0x18, // The segment is an executable code segment
    ACCESS_DATA_SEG      = 0x10, // The segment is a data segment
    ACCESS_SYSTEM_SEG    = 0x00, // The segment is a system segment like a task state segment

    ACCESS_RING_0        = 0 << 5, // The segment is in privilege level 0 (highest permissions)
    ACCESS_RING_1        = 1 << 5, // The segment is in privilege level 1
    ACCESS_RING_2        = 2 << 5, // The segment is in privilege level 2
    ACCESS_RING_3        = 3 << 5, // The segment is in privilege level 3 (lowest permission)

    ACCESS_PRESENT       = 0x80, // The segment is present - must be set for the segment ot be valid
};

enum GDTFlags {
    FLAG_64BIT           = 0x20, // The segment is a 64-bit segment
    FLAG_32BIT           = 0x40, // The segment is a 32-bit protected mode segment
    FLAG_16BIT           = 0x00, // The segment is a 16-bit protected mode segment

    FLAG_GRANULARITY_1B  = 0x00, // The limit is in 1 byte blocks
    FLAG_GRANULARITY_4K  = 0x80, // The limit is in 4 KiB blocks
};

// Create a GDT entry
#define CREATE_GDT_SEGMENT(base, limit, access, flags) { \
    (limit) & 0xFFFF, (base) & 0xFFFF, ((base) >> 16) & 0xFF, (access), (((limit) >> 16) & 0x0F) | ((flags) & 0xF0), ((base) >> 24) & 0xFF \
}

// Load the initial global descriptor table
void initialize_GDT();

#endif