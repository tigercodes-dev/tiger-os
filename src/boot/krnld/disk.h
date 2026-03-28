#ifndef KLD_DISK_H
#define KLD_DISK_H

#include <stdint.h>
#include <stdbool.h>

typedef struct {
    uint8_t id;
    uint16_t cyls;
    uint16_t secs;
    uint16_t heads;
} DISK;

// Sets up the basic information of a disk
bool load_disk(DISK* disk, uint8_t drive);

// Reads sectors from a disk
bool read_disk_sectors(DISK* disk, uint32_t lba, uint8_t sectors, void* data_out);

#endif