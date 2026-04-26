// TDISK - Tiger Disk Utility

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "stringutil.h"

#include "mbr.h"
#include "format.h"
#include "disk.h"

int main(int argc, char* argv[]) {
    if (argc < 2) {
        goto usage_error;
    }

    if (strcmp(argv[1], "--help") == 0) {
        // Open the help message in less
        if (access("/usr/local/share/doc/tdisk/tdisk-commands.txt", R_OK) == 0) {
            execl("/usr/bin/less", "less", "/usr/local/share/doc/tdisk/tdisk-commands.txt", NULL);
        } else if (access("/usr/share/doc/tdisk/tdisk-commands.txt", R_OK) == 0) {
            execl("/usr/bin/less", "less", "/usr/share/doc/tdisk/tdisk-commands.txt", NULL);
        } else {
            fprintf(stderr, "\e[31;1mError:\e[0m Help message documentation not found. Try installing to /usr or /usr/local.\n");
            return 1;
        }
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
