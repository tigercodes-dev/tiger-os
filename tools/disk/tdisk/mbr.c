#include "mbr.h"
#include "common.h"
#include "rand.h"
#include "string.h"
#include "diskutil.h"
#include "stringutil.h"
#include "mathutil.h"
#include <ctype.h>

int CMD_mbr(FILE* file, int argc, char* argv[]) {
    if (argc < 2) {
        printf("Usage: tdisk <file> %s <MBR-image>\nUse tdisk --help for more information.\n", argv[0]);
        return 1;
    }

    char* mbr_image_filepath = NULL;

    bool quiet = false;
    bool no_modify = false;
    bool del_part_table = false;

    for (int i = 1; i < argc; i++) {
        char* arg = argv[i];
        if (arg[0] == '-') {
            if (strcmp(arg, "-q") == 0 || strcmp(arg, "--quiet") == 0) {
                quiet = true;
            } else if (strcmp(arg, "-m") == 0 || strcmp(arg, "--no-modify") == 0) {
                no_modify = true;
            } else if (strcmp(arg, "-x") == 0 || strcmp(arg, "--del-partitions") == 0) {
                del_part_table = true;
            } else {
                fprintf(stderr, "\e[31;1mError:\e[0m Invalid option '%s'.\n", arg);
                return 1;
            }
        } else {
            if (mbr_image_filepath == NULL) {
                mbr_image_filepath = arg;
            } else {
                fprintf(stderr, "\e[31;1mError:\e[0m More than one MBR image file specified.\n");
                return 1;
            }
        }
    }

    if (mbr_image_filepath == NULL) {
        fprintf(stderr, "\e[31;1mError:\e[0m No MBR image file was specified.\n");
                return 1;
    }

    FILE* mbr_image = fopen(mbr_image_filepath, "rb");

    if (mbr_image == NULL) {
        fprintf(stderr, "\e[31;1mError:\e[0m The MBR image '%s' was not found.\n", mbr_image_filepath);
        return 2;
    }

    MBR new_mbr;
    if (fread(&new_mbr, 1, sizeof(MBR), mbr_image) < sizeof(MBR)) {
        fprintf(stderr, "\e[33;1mWarning:\e[0m The MBR image is smaller than typical size of %lu bytes.\n", sizeof(MBR));
    }

    if (new_mbr.boot_signature != 0xAA55 && !no_modify) {
        new_mbr.boot_signature = 0xAA55;
        printf("\e[36;1mNote:\e[0m The required boot signature was missing. It has been added.\n");
    }

    if (new_mbr.reserved == 0x0000 && !no_modify) {
        new_mbr.disk_signature = rand_32();
    }

    MBR old_mbr;

    fseek(file, 0, SEEK_SET);
    fread(&old_mbr, 1, sizeof(MBR), file);

    // Copy the old partition table
    if (!del_part_table && !no_modify) {
        memcpy(new_mbr.partition_table, old_mbr.partition_table, sizeof(old_mbr.partition_table));
    }

    if (!quiet) {
        printf("Do you want to write the MBR to the disk? This action may be irreversable! (Y/N) ");
        char confirm = 'n';
        scanf("%c", &confirm);
        if (tolower(confirm) != 'y') {
            printf("Operation canceled.\n");
            return 0;
        }
    }

    fseek(file, 0, SEEK_SET);
    if (fwrite(&new_mbr, 1, sizeof(MBR), file) < sizeof(MBR)) {
        fprintf(stderr, "\e[33;1mWarning:\e[0m Not all of the data was able to be written to the disk.\n");
    };

    fclose(mbr_image);
    
    return 0;
}

uint8_t parse_system_type(char* type) {
    if (stricmp(type, "None") == 0 || stricmp(type, "Empty") == 0) {
        return SYSTEM_TYPE_NONE;
    } else if (stricmp(type, "FAT12") == 0) {
        return SYSTEM_TYPE_FAT12;
    } else if (stricmp(type, "FAT16") == 0) {
        return SYSTEM_TYPE_FAT16;
    } else if (stricmp(type, "FAT16B") == 0) {
        return SYSTEM_TYPE_FAT16B;
    } else if (stricmp(type, "FAT32 CHS") == 0) {
        return SYSTEM_TYPE_FAT32CHS;
    } else {
        return SYSTEM_TYPE_NONE;
    }
}

static const PartitionTableEntry ZERO_ENTRY = {0};

int CMD_create_partition(FILE* file, int argc, char* argv[]) {
    if (argc < 2) {
        printf("Usage: tdisk <file> %s <partition-type>\nUse tdisk --help for more information.\n", argv[0]);
        return 1;
    }

    MBR mbr;
    fseek(file, 0, SEEK_SET);
    fread(&mbr, 1, sizeof(MBR), file);

    char* partition_type = NULL;
    uint32_t partition_size = 0; // Sectors in the partition
    uint32_t first_sector_offset = 0;
    uint8_t system_type = SYSTEM_TYPE_NONE;

    for (int i = 1; i < argc; i++) {
        char* arg = argv[i];
        if (arg[0] == '-') {
            if (strcmp(arg, "-s") == 0 || strcmp(arg, "--size") == 0) {
                char* size_arg = argv[++i];
                size_t size_bytes = parse_suffix_size(size_arg);
                partition_size = size_bytes / SECTOR_SIZE;
                if (size_bytes % SECTOR_SIZE != 0) partition_size++;
            } else if (strcmp(arg, "-f") == 0 || strcmp(arg, "--first") == 0) {
                char* first_arg = argv[++i];
                size_t first_bytes = parse_suffix_size(first_arg);
                first_sector_offset = first_bytes / SECTOR_SIZE;
                if (first_bytes % SECTOR_SIZE != 0) first_sector_offset++;
            } else if (strcmp(arg, "-t") == 0 || strcmp(arg, "--type") == 0) {
                char* type_arg = argv[++i];
                system_type = parse_system_type(type_arg);
            } else {
                fprintf(stderr, "\e[31;1mError:\e[0m Invalid option '%s'.\n", arg);
                return 1;
            }
        } else {
            if (partition_type == NULL) {
                partition_type = arg;
            } else {
                fprintf(stderr, "\e[31;1mError:\e[0m More than one partition type specified.\n");
                return 1;
            }
        }
    }

    int partition_number;

    if (stricmp(partition_type, "primary") == 0) {
        // Check for empty entries in the partition table.
        PartitionTableEntry* entry = NULL;
        uint32_t first_sector = 63;
        for (int i = 0; i < 4; i++) {
            if (memcmp(&mbr.partition_table[i], &ZERO_ENTRY, sizeof(PartitionTableEntry)) == 0) {
                if (entry == NULL) {
                    entry = &mbr.partition_table[i];
                    partition_number = i + 1;
                }
            } else {
                first_sector = max(first_sector, mbr.partition_table[i].start_lba + mbr.partition_table[i].partition_sectors);
            }
        }

        if (entry == NULL) {
            fprintf(stderr, "\e[31;1mError:\e[0m The primary partition table is full.\n");
            return 2;
        }

        first_sector += first_sector_offset;

        uint32_t disk_sectors = disk_sector_count(file);

        if (partition_size == 0) partition_size = disk_sectors - first_sector;

        if (first_sector + partition_size > disk_sectors) {
            fprintf(stderr, "\e[31;1mError:\e[0m There is not enough available disk space for the partition.\n");
            return 2;
        }

        entry->active = PARTITION_ACTIVE;
        entry->start_chs = lba_to_chs(first_sector, SECTORS_PER_TRACK, HEADS);
        entry->system_type = system_type;
        entry->end_chs = lba_to_chs(first_sector + partition_size - 1, SECTORS_PER_TRACK, HEADS);
        entry->start_lba = first_sector;
        entry->partition_sectors = partition_size;
    }

    // Write the partition to the disk
    fseek(file, 0, SEEK_SET);
    if (fwrite(&mbr, 1, sizeof(MBR), file) != sizeof(MBR)) {
        fprintf(stderr, "\e[33;1mWarning:\e[0m Not all of the data was able to be written to the disk.\n");
    }

    printf("Partition created.\nPartition number: %i.\n", partition_number);

    return 0;
}
