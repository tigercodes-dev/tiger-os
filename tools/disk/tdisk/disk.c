#include "disk.h"
#include "common.h"
#include <stdlib.h>

int CMD_read_sectors(FILE* file, int argc, char* argv[]) {
    if (argc < 3) {
        printf("Usage: tdisk <file> %s <lba-address> <count>\nUse tdisk --help for more information.\n", argv[0]);
        return 1;
    }

    long lba = -1;
    long count = -1;

    for (int i = 1; i < argc; i++) {
        char* arg = argv[i];
        char* endptr;
        if (lba == -1) {
            lba = strtoul(arg, &endptr, 0);
        } else if (count == -1) {
            count = strtoul(arg, &endptr, 0);
        }
    }

    if (lba == -1) {
        fprintf(stderr, "\e[31;1mError:\e[0m No LBA address was specified.\n");
        return 1;
    }
    if (count == -1) {
        fprintf(stderr, "\e[31;1mError:\e[0m The sector count was not specified.\n");
        return 1;
    }

    fseek(file, lba * SECTOR_SIZE, SEEK_SET);

    char buffer[SECTOR_SIZE];
    for (unsigned long i = 0; i < count; i++) {
        fread(buffer, 1, SECTOR_SIZE, file);
        fwrite(buffer, 1, SECTOR_SIZE, stdout);
    }

    return 0;
}

int CMD_write_sectors(FILE* file, int argc, char* argv[]) {
    if (argc < 3) {
        printf("Usage: tdisk <file> %s <lba-address> <count>\nUse tdisk --help for more information.\n", argv[0]);
        return 1;
    }

    long lba = -1;
    long count = -1;

    for (int i = 1; i < argc; i++) {
        char* arg = argv[i];
        char* endptr;
        if (lba == -1) {
            lba = strtoul(arg, &endptr, 0);
        } else if (count == -1) {
            count = strtoul(arg, &endptr, 0);
        }
    }

    if (lba == -1) {
        fprintf(stderr, "\e[31;1mError:\e[0m No LBA address was specified.\n");
        return 1;
    }
    if (count == -1) {
        fprintf(stderr, "\e[31;1mError:\e[0m The sector count was not specified.\n");
        return 1;
    }

    fseek(file, lba * SECTOR_SIZE, SEEK_SET);

    char buffer[SECTOR_SIZE];
    for (unsigned long i = 0; i < count; i++) {
        fread(buffer, 1, SECTOR_SIZE, stdin);
        fwrite(buffer, 1, SECTOR_SIZE, file);
    }

    return 0;
}
