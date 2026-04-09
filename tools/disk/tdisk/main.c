// TDISK - Tiger Disk Utility

#include <stdio.h>
#include <string.h>

#include "mbr.h"

#define VERSION "1.0.0"

int main(int argc, char* argv[]) {
    if (strcmp(argv[1], "--help") == 0) {
        printf("Usage: %s <disk-image> <subcmd> [args...].\n\n"
               "Commands:\n"
               "MBR Commands\n"
               "    mbr <mbr-image> [-q] [-x] [-m] - apply MBR binary image\n"
               "      -q, --quiet - Do not display confirmation prompt.\n"
               "      -x, --del-partitions - Do not attempt to save and restore partitions on the disk.\n"
               "      -m, --no-modify - Do not modify the MBR binary before applying.\n", argv[0]);
        return 0;
    }

    if (argc < 3) {
        printf("Usage: %s <file> <subcmd> [args...].\nUse %s --help to see more information.\n", argv[0], argv[0]);
        return 1;
    }

    printf("TDISK - Tiger Disk Utility v%s\nBy tigercodes-dev <https://github.com/tigercodes-dev>\nThis tool is included in TigerOS.\n\n", VERSION);

    char* filename = argv[1];
    char* subcmd = argv[2];

    int exit_code = 0;

    FILE* file = fopen(filename, "rb+");
    if (file == NULL) {
        fprintf(stderr, "\e[31;1mError:\e[0m The file '%s' was not found.\n", filename);
        return 2;
    }

    if (strcmp(subcmd, "mbr") == 0) {
        exit_code = CMD_mbr(file, argc - 2, &argv[2]);
    } else {
        fprintf(stderr, "\e[31;1mError:\e[0m The subcommand '%s' is invalid.\nUse %s --help to see all subcommands.\n", filename, argv[0]);
        fclose(file);
        return 1;
    }

    fclose(file);

    return exit_code;
}
