#include <stdint.h>
#include <stdbool.h>
#include "stdio.h"
#include "disk.h"
#include "fat16.h"

extern uint8_t __entry_start;

#define MEMORY_LOAD_KERNEL  ((void*)0x30000)
#define MEMORY_LOAD_SIZE    0x00010000
#define MEMORY_KERNEL_ADDR  ((void*)0x100000)

uint8_t* kernel_load_buffer = (uint8_t*)MEMORY_LOAD_KERNEL;
uint8_t* kernel = (uint8_t*)MEMORY_KERNEL_ADDR;

typedef void (*KernelStart)();

void __attribute__((cdecl)) krnld_start(uint8_t boot_drive) {
    clrscreen();
    printf("Loaded KRNLD.SYS at 0x%x\n", &__entry_start);
    
    puts("Initializing disk...\n");

    DISK disk;
    if (!load_disk(&disk, boot_drive)) {
        puts("Error: Unable to initialize the disk.\n");
        return;
    }

    puts("Initializing filesystem...\n");

    int error_code = fat16_init(&disk);
    if (error_code != 0) {
        printf("Error: Unable to initialize FAT16 filesystem.\nError Code: 0x%x\n", error_code);
        return;
    }

    puts("Loading kernel...\n");

    File* fd;
    error_code = fat16_open(&disk, "/system/test.txt", &fd);
    if (error_code != 0) {
        printf("Error: Unable to load the kernel.\nError Code: 0x%x\n", error_code);
        return;
    }
    
    uint32_t read;
    uint8_t* kernel_buffer = kernel;
    
    while (fat16_read(&disk, fd, MEMORY_LOAD_SIZE, kernel_load_buffer)) {
        puts(kernel_load_buffer);
    }

    fat16_close(fd);
}
