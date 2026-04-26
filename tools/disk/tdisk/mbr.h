#pragma once

#include <stdio.h>

int CMD_mbr(FILE* file, int argc, char* argv[]);

int CMD_create_partition(FILE* file, int argc, char* argv[]);

int CMD_list_partitions(FILE* file, int argc, char* argv[]);
