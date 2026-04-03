#ifndef _DESCRIPTOR_TABLE_H_
#define _DESCRIPTOR_TABLE_H_

#include "gdt.h"
#include "idt.h"
#include <stdint.h>

void __attribute__((cdecl)) load_gdt(GDTDescriptor* desc, uint16_t cs, uint16_t ds);
void __attribute__((cdecl)) load_idt(IDTDescriptor* desc);

#endif
