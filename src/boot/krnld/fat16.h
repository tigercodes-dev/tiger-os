#ifndef KLD_FAT16_H
#define KLD_FAT16_H

#include <stdint.h>
#include "disk.h"

typedef struct{
    uint8_t name[11];
    uint8_t attributes;
    uint8_t winnt_reserved;
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

int fat16_init(DISK* disk);
int fat16_open(DISK* disk, const char* path, File** fptr_out);
uint32_t fat16_read(DISK* disk, File* file, uint32_t byteCount, void* dataOut);
bool fat16_read_entry(DISK* disk, File* file, DirEntry* dirEntry);
void fat16_close(File* file);

#endif
