#include "fat16.h"
#include "math.h"
#include "memory.h"
#include "string.h"
#include "ctype.h"
#include "stdio.h"
#include <stddef.h>
#include <stdint.h>

#define SECTOR_SIZE      512
#define MAX_PATH         256
#define MAX_FILE_HANDLES 12
#define ROOT_DIR_HANDLE  -1

#define MEMORY_FAT_ADDR     ((void*)0x20000)
#define MEMORY_FAT_SIZE     0x00010000

typedef struct {
    uint8_t jmp_instruction[3];
    uint8_t oem[8];
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
    uint8_t winnt_flags;
    uint8_t signature;
    uint32_t volume_id;
    uint8_t volume_label[11];
    uint8_t filesystem_type[8];

} __attribute__((packed)) BootSector;

typedef struct {
    uint8_t buffer[SECTOR_SIZE];
    File public;
    bool opened;
    uint32_t first_cluster;
    uint32_t current_cluster;
    uint32_t current_sector_in_cluster;

} FileData;

typedef struct {
    union {
        BootSector boot_sector;
        uint8_t boot_sector_bytes[SECTOR_SIZE];
    } BS;

    FileData root_dir;
    FileData opened_files[MAX_FILE_HANDLES];
} FatData;

static FatData* fat_data;
static uint8_t* fat_bytes = NULL;
static uint32_t data_section_lba;

bool read_bs(DISK* disk) {
    return read_disk_sectors(disk, 0, 1, fat_data->BS.boot_sector_bytes);
}

bool read_fat(DISK* disk)
{
    return read_disk_sectors(disk, fat_data->BS.boot_sector.reserved_sectors, fat_data->BS.boot_sector.sectors_per_fat, fat_bytes);
}

int fat16_init(DISK* disk) {
    fat_data = (FatData*)MEMORY_FAT_ADDR;
    if (!read_bs(disk)) {
        return 0x0501;
    }

    fat_bytes = (uint8_t*)fat_data + sizeof(FatData);
    uint32_t fat_size = fat_data->BS.boot_sector.bytes_per_sector * fat_data->BS.boot_sector.sectors_per_fat;
    if (sizeof(FatData) + fat_size >= MEMORY_FAT_SIZE) {
        return 0x0502;
    }

    if (!read_fat(disk)) {
        return 0x0503;
    }

    uint32_t root_dir_lba = fat_data->BS.boot_sector.reserved_sectors + fat_data->BS.boot_sector.sectors_per_fat * fat_data->BS.boot_sector.fat_count;
    uint32_t root_dir_size = sizeof(DirEntry) * fat_data->BS.boot_sector.root_dir_entries;

    fat_data->root_dir.public.handle = ROOT_DIR_HANDLE;
    fat_data->root_dir.public.is_dir = true;
    fat_data->root_dir.public.position = 0;
    fat_data->root_dir.public.size = sizeof(DirEntry) * fat_data->BS.boot_sector.root_dir_entries;
    fat_data->root_dir.opened = true;
    fat_data->root_dir.first_cluster = root_dir_lba;
    fat_data->root_dir.current_cluster = root_dir_lba;
    fat_data->root_dir.current_sector_in_cluster = 0;

    if (!read_disk_sectors(disk, root_dir_lba, 1, fat_data->root_dir.buffer)) {
        return 0x0504;
    }

    uint32_t root_dir_sectors = (root_dir_size + fat_data->BS.boot_sector.bytes_per_sector - 1) / fat_data->BS.boot_sector.bytes_per_sector;
    data_section_lba = root_dir_lba + root_dir_sectors;

    for (int i = 0; i < MAX_FILE_HANDLES; i++) {
        fat_data->opened_files[i].opened = false;
    }

    return 0;
}

uint32_t cluster_to_lba(uint32_t cluster) {
    return data_section_lba + (cluster - 2) * fat_data->BS.boot_sector.sectors_per_cluster;
}

int open_entry(DISK* disk, DirEntry* entry, File** fileptr_out) {
    int handle = -1;
    for (int i = 0; i < MAX_FILE_HANDLES; i++) {
        if (!fat_data->opened_files[i].opened) {
            handle = i;
        }
    }

    if (handle == -1) {
        return 0x0511;
    }

    FileData* fd = &(fat_data->opened_files[handle]);
    fd->public.handle = handle;
    fd->public.is_dir = (entry->attributes & DIRECTORY) != 0;
    fd->public.position = 0;
    fd->public.size = entry->size;
    fd->first_cluster = entry->first_cluster_low | ((uint32_t)entry->first_cluster_high << 16);
    fd->current_cluster = fd->first_cluster;
    fd->current_sector_in_cluster = 0;

    if (!read_disk_sectors(disk, cluster_to_lba(fd->current_cluster), 1, fd->buffer)) {
        return 0x0512;
    }

    fd->opened = true;
    *fileptr_out = &fd->public;
    return 0;
}

uint32_t get_next_cluster(uint32_t current_cluster) {
    return *(uint16_t*)(fat_bytes + current_cluster * 2);
}

uint32_t fat16_read(DISK* disk, File* file, uint32_t byte_count, void* data_out) {
    FileData* fd = (file->handle == ROOT_DIR_HANDLE) ? &fat_data->root_dir : &fat_data->opened_files[file->handle];
    uint8_t* byte_data_out = (uint8_t*)data_out;

    if (!fd->public.is_dir || (fd->public.is_dir && fd->public.size != 0)) {
        byte_count = min(byte_count, fd->public.size - fd->public.position);
    }

    while (byte_count > 0) {
        uint32_t left = SECTOR_SIZE - (fd->public.position % SECTOR_SIZE);
        uint32_t take = min(byte_count, left);

        memcpy(byte_data_out, fd->buffer + fd->public.position % SECTOR_SIZE, take);
        byte_data_out += take;
        fd->public.position += take;
        byte_count -= take;

        if (left == take) {
            if (fd->public.handle == ROOT_DIR_HANDLE) {
                ++fd->current_cluster;

                if (!read_disk_sectors(disk, fd->current_cluster, 1, fd->buffer)) {
                    break;
                }
            } else {
                if (++fd->current_sector_in_cluster >= fat_data->BS.boot_sector.sectors_per_cluster) {
                    fd->current_sector_in_cluster = 0;
                    fd->current_cluster = get_next_cluster(fd->current_cluster);
                }

                if (fd->current_cluster >= 0xFFF8) {
                    fd->public.size = fd->public.position;
                    break;
                }

                if (!read_disk_sectors(disk, cluster_to_lba(fd->current_cluster) + fd->current_sector_in_cluster, 1, fd->buffer)) {
                    break;
                }
            }
        } 
    }

    return byte_data_out - (uint8_t*)data_out;
}

bool fat16_read_entry(DISK* disk, File* file, DirEntry* dir_entry) {
    return fat16_read(disk, file, sizeof(DirEntry), dir_entry) == sizeof(DirEntry);
}

void fat16_close(File* file) {
    if (file->handle == ROOT_DIR_HANDLE) {
        file->position = 0;
        fat_data->root_dir.current_cluster = fat_data->root_dir.first_cluster;
    } else {
        fat_data->opened_files[file->handle].opened = false;
    }
}

bool fat16_find_file(DISK* disk, File* file, const char* name, DirEntry* entry_out) {
    char fat_name[12];
    DirEntry entry;

    memset(fat_name, ' ', sizeof(fat_name));
    fat_name[11] = 0;

    const char* ext = strchr(name, '.');
    if (ext == NULL) {
        ext = name + 11;
    }

    for (int i = 0; i < 8 && name[i] && name + i < ext; i++) {
        fat_name[i] = toupper(name[i]);
    }

    if (ext != name + 11) {
        for (int i = 0; i < 3 && ext[i + 1]; i++) {
            fat_name[i + 8] = toupper(ext[i + 1]);
        } 
    }

    while (fat16_read_entry(disk, file, &entry)) {
        if (memcmp(fat_name, entry.name, 11) == 0) {
            *entry_out = entry;
            return true;
        }
    }

    return false;
}

int fat16_open(DISK* disk, const char* path, File** fptr_out) {
    char name[MAX_PATH];

    if (path[0] == '/') {
        path++;
    }

    File* current = &fat_data->root_dir.public;

    while (*path) {
        bool last = false;
        const char* delim = strchr(path, '/');

        if (delim != NULL) {
            memcpy(name, path, delim - path);
            name[delim - path + 1] = 0;
            path = delim + 1;
        } else {
            unsigned int len = strlen(path);
            memcpy(name, path, len);
            name[len + 1] = 0;
            path += len;
            last = true;
        }

        DirEntry entry;
        if (fat16_find_file(disk, current, name, &entry)) {
            fat16_close(current);

            if (!last && entry.attributes & DIRECTORY == 0) {
                *fptr_out = NULL;
                return 0x0531;
            }

            int error = open_entry(disk, &entry, &current);
            if (error != 0) {
                *fptr_out = NULL;
                return error;
            }
        } else {
            fat16_close(current);
            *fptr_out = NULL;
            return 0x0532;
        }
    }

    *fptr_out = current;
    return 0;
}
