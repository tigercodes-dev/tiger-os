#include "mathutil.h"

int min(int a, int b) {
    return a < b ? a : b;
}

int max(int a, int b) {
    return a > b ? a : b;
}

int cdiv(int a, int b) {
    int q = a / b;
    if (a % b != 0) q++;
    return q;
}

int align(int a, int b) {
    return cdiv(a, b) * b;
}

int alignpow2(int x) {
    if (x == 0) return 1;
    x--;
    x |= x >> 1;
    x |= x >> 2;
    x |= x >> 4;
    x |= x >> 8;
    x |= x >> 16;
    return x + 1;
}
