#include <stdint.h>
#include <stdbool.h>
#include "stdio.h"
#include "disk.h"
#include "fat32.h"
#include "memory.h"

extern uint8_t __entry_start;

#define MEMORY_LOAD_KERNEL  ((void*)0x30000)
#define MEMORY_LOAD_SIZE    0x00010000
#define MEMORY_KERNEL_ADDR  ((void*)0x100000)

uint8_t* kernel_load_buffer = (uint8_t*)MEMORY_LOAD_KERNEL;
uint8_t* kernel = (uint8_t*)MEMORY_KERNEL_ADDR;

typedef void (*KernelStart)();

void print_error_msg(int error_code);

void __attribute__((cdecl)) krnld_start(uint8_t boot_drive) {
    clrscreen();
    printf("Loaded KRNLD.SYS at 0x%x\n", &__entry_start);
    
    puts("Initializing disk...\n");

    DISK disk;
    if (!load_disk(&disk, boot_drive)) {
        puts("Error: Unable to initialize the disk.\n");
        goto quit;
    }

    puts("Initializing filesystem...\n");

    int error_code = fat32_init(&disk);
    if (error_code != 0) {
        printf("Error: Unable to initialize FAT32 filesystem.\nError Code: 0x%x\n", error_code);
        print_error_msg(error_code);
        goto quit;
    }

    puts("Loading kernel...\n");

    File* fd;
    error_code = fat32_open(&disk, "/system/kernel.sys", &fd);
    if (error_code != 0) {
        printf("Error: Unable to load the kernel.\nError Code: 0x%x\n", error_code);
        print_error_msg(error_code);
        goto quit;
    }
    
    uint32_t read;
    uint8_t* kernel_buffer = kernel;
    
    while ((read = fat32_read(&disk, fd, MEMORY_LOAD_SIZE, kernel_load_buffer))) {
        memcpy(kernel_buffer, kernel_load_buffer, read);
        kernel_buffer += read;
    }

    fat32_close(fd);

    KernelStart kernel_entry = (KernelStart)kernel;
    kernel_entry();

    quit:

    puts("KRNLD exited. Press any key to reboot...");
    return;
}

void print_error_msg(int error_code) {
    switch (error_code) {
        // 0x0500 - Initialization Error
        case 0x0501:
            puts("Unable to read the boot sector.\n");
            break;
        case 0x0502:
            puts("There is not enough memory for reading the File Allocation Table.\n");
            break;
        case 0x0503:
            puts("Unable to read the File Allocation Table.\n");
            break;
        case 0x0504:
            puts("Unable to read the root directory.\n");
            break;
        // 0x0510 - Opening File Entry Error
        case 0x0511:
            puts("No available file handles.\n");
            break;
        case 0x0512:
            puts("Disk read error occured.\n");
            break;
        // 0x0530 - Find File Error
        case 0x0531:
        case 0x0532:
            puts("/SYSTEM/KERNEL.SYS was not found.\n");
    }
}
