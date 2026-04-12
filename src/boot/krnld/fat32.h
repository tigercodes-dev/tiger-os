#ifndef KLD_FAT32_H
#define KLD_FAT32_H

#include <stdint.h>
#include "disk.h"

typedef struct {
    uint8_t name[11];
    uint8_t attributes;
    uint8_t __reserved;
    uint8_t creation_time_hundredths;
    uint16_t creation_time;
    uint16_t creation_date;
    uint16_t accessed_date;
    uint16_t first_cluster_high;
    uint16_t modified_time;
    uint16_t modified_date;
    uint16_t first_cluster_low;
    uint32_t size;
} __attribute__((packed)) DirEntry;

typedef struct {
    int handle;
    bool is_dir;
    uint32_t position;
    uint32_t size;
} File;

enum FatAttributes {
    READ_ONLY = 0x01,
    HIDDEN    = 0x02,
    SYSTEM    = 0x04,
    VOLUME_ID = 0x08,
    DIRECTORY = 0x10,
    ARCHIVE   = 0x20,
    LFN = READ_ONLY | HIDDEN | SYSTEM | VOLUME_ID,
};

int fat32_init(DISK* disk);
int fat32_open(DISK* disk, const char* path, File** fptr_out);
uint32_t fat32_read(DISK* disk, File* file, uint32_t byte_count, void* data_out);
bool fat32_read_entry(DISK* disk, File* file, DirEntry* dirEntry);
void fat32_close(File* file);

#endif
