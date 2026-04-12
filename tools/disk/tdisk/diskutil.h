#pragma once

#include <stdio.h>
#include "common.h"

unsigned int disk_sector_count(FILE* file);

CHS lba_to_chs(uint32_t lba, int sectors_per_track, int heads);

size_t parse_suffix_size(const char* str);
