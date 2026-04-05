#include "stdio.h"
#include <stdarg.h>
#include <stdbool.h>

void fputc(char c, fd_t file) {
    fd_write(file, &c, sizeof(c));
}

void fputs(const char* s, fd_t file) {
    for (; *s; s++) {
        fputc(*s, file);
    }
}

typedef enum {
    STATE_NORMAL = 0,
    STATE_LEN    = 1,
    STATE_LEN_H  = 2,
    STATE_LEN_L  = 3,
    STATE_SPEC   = 4,
} PRINTF_State;

typedef enum {
    LENGTH_DEF   = 0,
    LENGTH_HH    = 1,
    LENGTH_H     = 2,
    LENGTH_L     = 3,
    LENGTH_LL    = 4,
} PRINTF_Length;

const char HEX_CHARS[]       = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'a', 'b', 'c', 'd', 'e', 'f'};

void fprint_uint(fd_t file, unsigned long long number, int base) {
    char buffer[32];
    int pos = 0;

    do {
        unsigned long long rem = number % base;
        number /= base;
        buffer[pos++] = HEX_CHARS[rem];
    } while (number > 0);

    while (--pos >= 0) {
        fputc(buffer[pos], file);
    }
}

void fprint_int(fd_t file, signed long long number, int base) {
    if (number < 0) {
        fputc('-', file);
        fprint_uint(file, -number, base);
    } else {
        fprint_uint(file, number, base);
    }
}

void vfprintf(fd_t file, const char* fmt, va_list args) {
    PRINTF_State state = STATE_NORMAL;
    PRINTF_Length length = LENGTH_DEF;
    int base = 10;
    bool sign = false;
    bool num = false;

    for (; *fmt; fmt++) {
        switch (state) {
            case STATE_NORMAL:
                if (*fmt == '%') {
                    state = STATE_LEN;
                } else {
                    fputc(*fmt, file);
                }
                break;

            case STATE_LEN:
                switch (*fmt) {
                    case 'h':
                        length = LENGTH_H;
                        state = STATE_LEN_H;
                        break;
                    case 'l':
                        length = LENGTH_L;
                        state = STATE_LEN_L;
                        break;
                    default:
                        goto SPEC;
                }
                break;
            
            case STATE_LEN_H:
                if (*fmt == 'h') {
                    length = LENGTH_HH;
                    state = STATE_SPEC;
                } else {
                    goto SPEC;
                }
                break;

            case STATE_LEN_L:
                if (*fmt == 'l') {
                    length = LENGTH_LL;
                    state = STATE_SPEC;
                } else {
                    goto SPEC;
                }
                break;

            case STATE_SPEC:
            SPEC:
                switch (*fmt) {
                    case 'c':
                        fputc((char)va_arg(args, int), file);
                        break;
                    case 's':
                        fputs(va_arg(args, const char*), file);
                        break;
                    case '%':
                        fputc('%', file);
                        break;
                    case 'd':
                    case 'i':
                        base = 10;
                        sign = true;
                        num = true;
                        break;
                    case 'u':
                        base = 10;
                        sign = false;
                        num = true;
                        break;
                    case 'x':
                    case 'X':
                    case 'p':
                        base = 16;
                        sign = false;
                        num = true;
                        break;
                    case 'o':
                        base = 8;
                        sign = false;
                        num = true;
                        break;
                }

                if (num) {
                    if (sign) {
                        switch (length) {
                            case LENGTH_HH:
                            case LENGTH_H:
                            case LENGTH_DEF:
                                fprint_int(file, va_arg(args, int), base);
                                break;
                            case LENGTH_L:
                                fprint_int(file, va_arg(args, long), base);
                                break;
                            case LENGTH_LL:
                                fprint_int(file, va_arg(args, long long), base);
                                break;
                        }
                    } else {
                        switch (length) {
                            case LENGTH_HH:
                            case LENGTH_H:
                            case LENGTH_DEF:
                                fprint_uint(file, va_arg(args, unsigned int), base);
                                break;
                            case LENGTH_L:
                                fprint_uint(file, va_arg(args, unsigned long), base);
                                break;
                            case LENGTH_LL:
                                fprint_uint(file, va_arg(args, unsigned long long), base);
                                break;
                        }
                    }
                }
                
                state = STATE_NORMAL;
                length = LENGTH_DEF;
                base = 10;
                sign = false;
                num = false;
                break;
        }
    }
}

void fprintf(fd_t file, const char* fmt, ...) {
    va_list args;
    va_start(args, fmt);
    vfprintf(file, fmt, args);
    va_end(args);
}

void putc(char c) {
    fputc(c, STDOUT);
}

void puts(const char* s) {
    fputs(s, STDOUT);
}

void print_uint(unsigned long long number, int base) {
    fprint_uint(STDOUT, number, base);
}

void print_int(signed long long number, int base) {
    fprint_int(STDOUT, number, base);
}

void printf(const char* fmt, ...) {
    va_list args;
    va_start(args, fmt);
    vfprintf(STDOUT, fmt, args);
    va_end(args);
}
