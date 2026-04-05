#include "vfs.h"
#include "../i686/vga.h"
#include "../i686/debug.h"

size_t fd_write(fd_t file, uint8_t* data, size_t size) {
    switch (file) {
        case STDIN:
        default:
            return 0; // Can't write to input
        case STDOUT:
        case STDERR:
            for (size_t i = 0; i < size; i++) {
                putc_VGA(data[i]);
            }
            return size;
        case STDDBG:
            for (size_t i = 0; i < size; i++) {
                putc_dbg(data[i]);
            }
            return size;
    }
}