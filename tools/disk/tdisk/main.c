// TDISK - Tiger Disk Utility

#include <stdio.h>
#include <string.h>
#include "stringutil.h"

#include "mbr.h"
#include "format.h"
#include "disk.h"

int main(int argc, char* argv[]) {
    if (argc < 2) {
        goto usage_error;
    }

    if (strcmp(argv[1], "--help") == 0) {
        printf("Usage: %s <disk-image> <subcmd> [args...].\n\n"
               "Commands:\n"
               "Disk Commands\n"
               "    read-sectors <lba-address> <count> [-o <file>]\n"
               "      -o, --output <file> - Outputs read data to a file instead of stdout.\n"
               "    write-sectors <lba-address> <count> [-i <file>]\n"
               "      -i, --input <file> - Read data to write from a file instead of stdin.\n"
               "Partitioning Commands\n"
               "    mbr <mbr-image> [-q] [-x] [-m] - apply MBR binary image\n"
               "      -q, --quiet - Do not display confirmation prompt.\n"
               "      -x, --del-partitions - Do not attempt to save and restore partitions on the disk.\n"
               "      -m, --no-modify - Do not modify the MBR binary before applying.\n"
               "    create-partition <type> [-s <size>] [-f <offset>] [-t <type>] - create a new partition\n"
               "      Note: Currently only primary partitions are supported. Type must be 'primary'.\n"
               "      -s, --size <size> - Sets the size of the partition, if not specified, partition takes the remaining disk space.\n"
               "      -f, --first <offset> - Adds an offset between the new partition and the last.\n"
               "      -t, --type <type> - Sets the partition system id/type.\n"
               "\n"
               "TDISK is included with TigerOS by tigercodes-dev <https://github.com/tigercodes-dev/tiger-os>.\n", argv[0]);
        return 0;
    } else if (strcmp(argv[1], "--version") == 0) {
        printf("TDISK Version 0.1.0\n"
               "TigerOS Version 0.2.1\n"
               "TDISK is included with TigerOS by tigercodes-dev <https://github.com/tigercodes-dev/tiger-os>\n");
        return 0;
    }

    if (argc < 3) {
    usage_error:
        printf("Usage: %s <file> <subcmd> [args...].\nUse %s --help to see more information.\n", argv[0], argv[0]);
        return 1;
    }

    char* filename = argv[1];
    char* subcmd = argv[2];

    int exit_code = 0;

    FILE* file = fopen(filename, "rb+");
    if (file == NULL) {
        fprintf(stderr, "\e[31;1mError:\e[0m The file '%s' was not found.\n", filename);
        return 2;
    }

    if (stricmp(subcmd, "mbr") == 0) {
        exit_code = CMD_mbr(file, argc - 2, &argv[2]);
    } else if (stricmp(subcmd, "create-partition") == 0) {
        exit_code = CMD_create_partition(file, argc - 2, &argv[2]);
    } else if (stricmp(subcmd, "list-partitions") == 0) {
        exit_code = CMD_list_partitions(file, argc - 2, &argv[2]);
    } else if (stricmp(subcmd, "set-active") == 0) {
        exit_code = CMD_set_active(file, argc - 2, &argv[2]);
    } else if (stricmp(subcmd, "delete-partition") == 0) {
        exit_code = CMD_delete_partition(file, argc - 2, &argv[2]);
    } else if (stricmp(subcmd, "format") == 0) {
        exit_code = CMD_format(file, argc - 2, &argv[2]);
    } else if (stricmp(subcmd, "read-sectors") == 0) {
        exit_code = CMD_read_sectors(file, argc - 2, &argv[2]);
    } else if (stricmp(subcmd, "write-sectors") == 0) {
        exit_code = CMD_write_sectors(file, argc - 2, &argv[2]);
    } else {
        fprintf(stderr, "\e[31;1mError:\e[0m The subcommand '%s' is invalid.\nUse %s --help to see all subcommands.\n", filename, argv[0]);
        fclose(file);
        return 1;
    }

    fclose(file);

    return exit_code;
}
