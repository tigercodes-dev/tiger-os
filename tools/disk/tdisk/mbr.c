#include "mbr.h"
#include "common.h"
#include "rand.h"
#include "string.h"
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
        fprintf(stderr, "\e[33;1mWarning:\e[0m Not all of the data was written to the disk.\n");
    };

    fclose(mbr_image);
    
    return 0;
}
