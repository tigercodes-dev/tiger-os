#include "format.h"
#include "fs/fat/fat12.h"
#include "common.h"
#include <string.h>
#include <stdlib.h>

int CMD_format(FILE* file, int argc, char* argv[]) {
    if (argc < 2) {
        printf("Usage: tdisk <file> %s <partition-num>\nUse tdisk --help for more information.\n", argv[0]);
        return 1;
    }

    bool quick = false;
    char* volume_label = "TIGERDISK";
    int partition_num = -1;

    for (int i = 1; i < argc; i++) {
        char* arg = argv[i];
        if (arg[0] == '-') {
            if (strcmp(arg, "-q") == 0 || strcmp(arg, "--quick") == 0) {
                quick = true;
            } else if (strcmp(arg, "-v") == 0 || strcmp(arg, "--vol-label")) {
                volume_label = argv[++i];
            } else {
                fprintf(stderr, "\e[31;1mError:\e[0m Invalid option '%s'.\n", arg);
                return 1;
            }
        } else {
            if (partition_num < 0) {
                char* endptr;
                partition_num = strtoul(argv[i], &endptr, 10) - 1;
                if (partition_num < 0 || partition_num > 3) {
                    fprintf(stderr, "\e[31;1mError:\e[0m Invalid partition number: %i.\n", partition_num + 1);
                    return 1;
                }
            }
        }
    }

    if (partition_num < 0) {
        fprintf(stderr, "\e[31;1mError:\e[0m No partition number specified.\n");
        return 1;
    }

    fseek(file, 0, SEEK_SET);
    MBR mbr;
    fread(&mbr, 1, sizeof(MBR), file);

    PartitionTableEntry* entry = &mbr.partition_table[partition_num];
    switch (entry->system_type) {
        case SYSTEM_TYPE_NONE:
            fprintf(stderr, "\e[31;1mError:\e[0m The partition has no type. You must manually specify it.\n");
            return 2;
        case SYSTEM_TYPE_FAT12:
            return FAT12_format(file, entry->start_lba, entry->partition_sectors, quick, volume_label);
        default:
            fprintf(stderr, "\e[31;1mError:\e[0m The partition does not have a supported type. You must manually specify it.\n");
            return 2;
    }
}
