#include "diskutil.h"
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

unsigned int disk_sector_count(FILE* file) {
    long old_pos = ftell(file);
    fseek(file, 0, SEEK_END);
    long size = ftell(file);
    fseek(file, old_pos, SEEK_SET);
    
    int sectors = size / SECTOR_SIZE;
    if (size % SECTOR_SIZE != 0) sectors++;

    return sectors;
}

CHS lba_to_chs(uint32_t lba, int sectors_per_track, int heads) {
    CHS chs_out;
    chs_out.sec_cyl = ((lba % sectors_per_track + 1) & 0x003F) | ((lba / sectors_per_track / heads) << 6);
    chs_out.head = (lba / sectors_per_track) % heads;
    return chs_out;
}

size_t parse_suffix_size(const char* str) {
    char* endptr;
    size_t base = strtoul(str, &endptr, 0);

    switch (toupper(*endptr)) {
        case 'S':
            return base * SECTOR_SIZE;
        case 'K':
            return base * 1024;
        case 'M':
            return base * 1024 * 1024;
        case 'G':
            return base * 1024 * 1024 * 1024;
        case 'T':
            return base * 1024 * 1024 * 1024 * 1024;
        default:
            return base;
    }
}
