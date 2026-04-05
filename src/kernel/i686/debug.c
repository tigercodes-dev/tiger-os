#include "debug.h"
#include "ports.h"

#define DEBUG_PORT 0xE9

void putc_dbg(char c) {
    outb(DEBUG_PORT, c); 
}


