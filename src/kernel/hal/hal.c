#include "hal.h"
#include "../i686/gdt.h"

// Initialize the Hardware Abstraction Layer
void initialize_HAL() {
    initialize_GDT();
}
