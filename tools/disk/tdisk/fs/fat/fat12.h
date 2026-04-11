#pragma once

#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>

int FAT12_format(FILE* file, uint32_t partition_start, uint32_t partition_size, bool quick, const char* vol_label);
