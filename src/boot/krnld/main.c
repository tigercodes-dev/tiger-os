#include <stdint.h>
#include <stdbool.h>
#include "stdio.h"

void __attribute__((cdecl)) krnld_start(uint8_t boot_drive) {
    clrscreen();
    puts("Loading TigerOS...\n");

    while (true);
}
