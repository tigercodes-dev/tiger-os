#include "mbr.h"
#include "common.h"
#include "rand.h"
#include "string.h"
#include "diskutil.h"
#include "stringutil.h"
#include "mathutil.h"
#include <ctype.h>

static bool read_mbr(FILE* disk, void* buffer) {
    long oldpos = ftell(disk);

    fseek(disk, 0, SEEK_SET);
    bool success = fread(buffer, 1, sizeof(MBR), disk) == sizeof(MBR);

    fseek(disk, oldpos, SEEK_SET);

    return success;
}

static bool write_mbr(FILE* disk, void* buffer) {
    long oldpos = ftell(disk);

    fseek(disk, 0, SEEK_SET);
    bool success = fwrite(buffer, 1, sizeof(MBR), disk) == sizeof(MBR);

    fseek(disk, oldpos, SEEK_SET);

    return success;
}

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
        fprintf(stderr, "\e[31;1mError:\e[0m The MBR image is smaller than sector size of %lu bytes.\n", sizeof(MBR));
        return 2;
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
    if (!read_mbr(file, &old_mbr)) {
        fprintf(stderr, "\e[33;1mWarning:\e[0m Could not read the old MBR. Partition table saving will be disabled.\n");
        del_part_table = true;
    }

    // Copy the old partition table
    if (!no_modify) {
        if (del_part_table) {
            memset(new_mbr.partition_table, 0, sizeof(new_mbr.partition_table));
        } else {
            memcpy(new_mbr.partition_table, old_mbr.partition_table, sizeof(old_mbr.partition_table));
        }
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
    if (!write_mbr(file, &new_mbr)) {
        fprintf(stderr, "\e[33;1mWarning:\e[0m Not all of the data was able to be written to the disk.\n");
    };

    fclose(mbr_image);
    
    return 0;
}

// Forward Slashes (/) separate numbers with multiple types.
static const char* const SYSTEM_TYPES[] = {
    [SYSTEM_TYPE_NONE] = "Empty/None",
    [SYSTEM_TYPE_FAT12] = "FAT12",
    [SYSTEM_TYPE_XNXRT] = "XENIX root",
    [SYSTEM_TYPE_XNXUSR] = "XENIX usr",
    [SYSTEM_TYPE_FAT16] = "FAT16",
    [SYSTEM_TYPE_EXTD] = "Extended",
    [SYSTEM_TYPE_FAT16B] = "FAT16B",
    [SYSTEM_TYPE_HPNTEXF] = "HPFS/NTFS/exFAT",
    [SYSTEM_TYPE_AIX] = "AIX",
    [SYSTEM_TYPE_AIXBT] = "AIX bootable/Coherent",
    [SYSTEM_TYPE_OS2BTMGR] = "OS2 Boot Manager",
    [SYSTEM_TYPE_FAT32CHS] = "FAT32",
    [SYSTEM_TYPE_FAT32LBA] = "FAT32 LBA",
    [SYSTEM_TYPE_FAT16LBA] = "FAT16B LBA",
    [SYSTEM_TYPE_EXTDLBA] = "Extended LBA",
    [SYSTEM_TYPE_H_FAT12] = "Hidden FAT12",
    [SYSTEM_TYPE_CFGSVC] = "Compaq Configuration/Service Partition",
    [SYSTEM_TYPE_H_FAT16] = "Hidden FAT16",
    [SYSTEM_TYPE_H_EXTD] = "Hidden Extended",
    [SYSTEM_TYPE_H_FAT16B] = "Hidden FAT16B",
    [SYSTEM_TYPE_H_HPNTEX] = "Hidden HPFS/Hidden NTFS/Hidden exFAT",
    [SYSTEM_TYPE_H_F32CHS] = "Hidden FAT32",
    [SYSTEM_TYPE_H_F32LBA] = "Hidden FAT32 LBA",
    [SYSTEM_TYPE_H_F16LBA] = "Hidden FAT16B LBA",
    [SYSTEM_TYPE_H_EXTLBA] = "Hidden Extended LBA",
    [SYSTEM_TYPE_NECDOS] = "NEC DOS",
    [SYSTEM_TYPE_WINRE] = "WinRE",
    [SYSTEM_TYPE_AIXJFS] = "AIX JFS",
    [SYSTEM_TYPE_PLAN9] = "Plan 9",
    [SYSTEM_TYPE_PMREC] = "PartitionMagic Recovery",
    [SYSTEM_TYPE_PREPBOOT] = "PPC PReP Boot",
    [SYSTEM_TYPE_SFS] = "SFS",
    [SYSTEM_TYPE_QNX4_1] = "QNX4 Primary",
    [SYSTEM_TYPE_QNX4_2] = "QNX4 Secondary",
    [SYSTEM_TYPE_QNX4_3] = "QNX4 Tertiary",
    [SYSTEM_TYPE_GNUHURD] = "GNU Hurd",
    [SYSTEM_TYPE_MINIXOLD] = "Minix Old",
    [SYSTEM_TYPE_MINIX] = "Minix",
    [SYSTEM_TYPE_LSWP_SLS] = "Linux Swap/Solaris Old",
    [SYSTEM_TYPE_LINUX] = "Linux",
    [SYSTEM_TYPE_LINEXTD] = "Linux Extended",
    [SYSTEM_TYPE_LINPLTXT] = "Linux Plaintext",
    [SYSTEM_TYPE_AMOEBA] = "Amoeba",
    [SYSTEM_TYPE_BSDOS] = "BSD OS",
    [SYSTEM_TYPE_FREEBSD] = "FreeBSD",
    [SYSTEM_TYPE_OPENBSD] = "OpenBSD",
    [SYSTEM_TYPE_DWNUFS] = "Darwin UFS",
    [SYSTEM_TYPE_DWNBOOT] = "Darwin Boot",
    [SYSTEM_TYPE_HFS] = "HFS/HFS+",
    [SYSTEM_TYPE_SLS_BOOT] = "Solaris Boot",
    [SYSTEM_TYPE_SOLARIS] = "Solaris",
    [SYSTEM_TYPE_DELLUTIL] = "Dell Utility",
    [SYSTEM_TYPE_GPT] = "GPT",
    [SYSTEM_TYPE_EFI] = "EFI/EFI System",
    [0xFF] = NULL
};

uint8_t parse_system_type(char* type) {
    for (int i = 0; i < 256; i++) {
        char* type_val = (char*)SYSTEM_TYPES[i];
        if (type_val == NULL) continue;
        char namebuf[64] = {0};
        strcpy(namebuf, type_val);

        char* token = strtok(namebuf, "/");

        while (token != NULL) {
            if (stricmp(token, type) == 0) return i;
            token = strtok(NULL, "/");
        }
    }
    return SYSTEM_TYPE_NONE;
}

static const PartitionTableEntry ZERO_ENTRY = {0};

int CMD_create_partition(FILE* file, int argc, char* argv[]) {
    if (argc < 2) {
        printf("Usage: tdisk <file> %s <partition-type>\nUse tdisk --help for more information.\n", argv[0]);
        return 1;
    }

    MBR mbr;
    fseek(file, 0, SEEK_SET);
    if (!read_mbr(file, &mbr)) {
        fprintf(stderr, "\e[31;1mError:\e[0m Unable to read the MBR.\n");
        return 1;
    }

    char* partition_type = NULL;
    uint32_t partition_size = 0; // Sectors in the partition
    uint32_t first_sector_offset = 0;
    uint8_t system_type = SYSTEM_TYPE_NONE;

    for (int i = 1; i < argc; i++) {
        char* arg = argv[i];
        if (arg[0] == '-') {
            if (strcmp(arg, "-s") == 0 || strcmp(arg, "--size") == 0) {
                if (i + 1 >= argc) {
                    fprintf(stderr, "\e[31;1mError:\e[0m No size argument was specified.\n");
                    return 1;
                }
                char* size_arg = argv[++i];
                size_t size_bytes = parse_suffix_size(size_arg);
                partition_size = size_bytes / SECTOR_SIZE;
                if (size_bytes % SECTOR_SIZE != 0) partition_size++;
            } else if (strcmp(arg, "-f") == 0 || strcmp(arg, "--first") == 0) {
                if (i + 1 >= argc) {
                    fprintf(stderr, "\e[31;1mError:\e[0m No offset argument was specified.\n");
                    return 1;
                }
                char* first_arg = argv[++i];
                size_t first_bytes = parse_suffix_size(first_arg);
                first_sector_offset = first_bytes / SECTOR_SIZE;
                if (first_bytes % SECTOR_SIZE != 0) first_sector_offset++;
            } else if (strcmp(arg, "-t") == 0 || strcmp(arg, "--type") == 0) {
                if (i + 1 >= argc) {
                    fprintf(stderr, "\e[31;1mError:\e[0m No type argument was specified.\n");
                    return 1;
                }
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
    if (!write_mbr(file, &mbr)) {
        fprintf(stderr, "\e[33;1mWarning:\e[0m Not all of the data was able to be written to the disk.\n");
    }

    printf("Partition created.\nPartition number: %i.\n", partition_number);

    return 0;
}

static void print_with_suffix(size_t size) {
    if (size < 1024ul) {
        printf("%dB", (unsigned int)size);
    } else if (size < 1024ul * 1024ul) {
        printf("%.1fK", size / 1024.0f);
    } else if (size < 1024ul * 1024ul * 1024ul) {
        printf("%.1fM", size / (1024.0f * 1024.0f));
    } else if (size < 1024ul * 1024ul * 1024ul * 1024ul) {
        printf("%.1fB", size / (1024.0f * 1024.0f * 1024.0f));
    } else {
        printf("%.1fT", size / (1024.0f * 1024.0f * 1024.0f * 1024.0f));
    }
}

int CMD_list_partitions(FILE* file, int argc, char* argv[]) {
    MBR mbr;
    if (!read_mbr(file, &mbr)) {
        fprintf(stderr, "\e[31;1mError:\e[0m Unable to read the MBR.\n");
        return 1;
    }

    printf("Num  Type       Boot   System Type     First      Last      Size\n");

    bool partition_found = false;
    for (int i = 0; i < 4; i++) {
        if (mbr.partition_table[i].partition_sectors > 0) {
            char part_num[5];
            snprintf(part_num, 4, "%d.", i + 1);
            printf("%-4s %-10s %-6c %-15s %-10d %-10d", part_num, "Primary", mbr.partition_table[i].active & 0x80 ? '*' : ' ',
                SYSTEM_TYPES[mbr.partition_table[i].system_type], mbr.partition_table[i].start_lba,
                mbr.partition_table[i].start_lba + mbr.partition_table[i].partition_sectors - 1);
            print_with_suffix(mbr.partition_table[i].partition_sectors * SECTOR_SIZE);
            putchar('\n');
            partition_found = true;
        }
    }

    if (!partition_found) {
        printf("\nNo partitions found.\n");
        return 1;
    }

    return 0;
}
