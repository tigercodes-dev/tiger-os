#include "gdt.h"
#include "descriptor-table.h"

GDTSegment gdt[] = {
    // Null Descriptor
    CREATE_GDT_SEGMENT(0, 0, 0, 0),

    // Kernel 32-bit Code Segment
    CREATE_GDT_SEGMENT(0, 0xFFFFF, ACCESS_PRESENT | ACCESS_RING_0 | ACCESS_CODE_SEG | ACCESS_CS_READABLE, FLAG_32BIT | FLAG_GRANULARITY_4K),

    // Kernel 32-bit Data Segment
    CREATE_GDT_SEGMENT(0, 0xFFFFF, ACCESS_PRESENT | ACCESS_RING_0 | ACCESS_DATA_SEG | ACCESS_DS_WRITABLE, FLAG_32BIT | FLAG_GRANULARITY_4K),
};

GDTDescriptor descriptor = { sizeof(gdt) - 1, gdt};

void initialize_GDT() {
    load_gdt(&descriptor, 0x08, 0x10);
}
