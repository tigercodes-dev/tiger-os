#include "rand.h"
#include <stdio.h>

void randbytes(void* ptr, unsigned int count) {
    FILE* urandom = fopen("/dev/urandom", "rb");
    fread(ptr, 1, count, urandom);
    fclose(urandom);
}

uint8_t rand_8() {
    uint8_t value;
    randbytes(&value, 1);
    return value;
}

uint16_t rand_16() {
    uint16_t value;
    randbytes(&value, 2);
    return value;
}

uint32_t rand_32() {
    uint32_t value;
    randbytes(&value, 4);
    return value;
}
