#include "stdio.h"
#include "i686/ports.h"
#include <stdarg.h>

#define SCREEN_WIDTH (int)80
#define SCREEN_HEIGHT (int)25
#define DEFAULT_COLOR 0x07

uint8_t* SCREEN_BUFFER = (uint8_t*)0xB8000;

int pos_x = 0, pos_y = 0;

void set_screen_chr(int x, int y, char c) {
    SCREEN_BUFFER[2 * (y * SCREEN_WIDTH + x)] = c;
}

void set_screen_color(int x, int y, uint8_t color) {
    SCREEN_BUFFER[2 * (y * SCREEN_WIDTH + x) + 1] = color;
}

char get_screen_chr(int x, int y) {
    return SCREEN_BUFFER[2 * (y * SCREEN_WIDTH + x)];
}

uint8_t get_screen_color(int x, int y) {
    return SCREEN_BUFFER[2 * (y * SCREEN_WIDTH + x) + 1];
}

void move_curs(int x, int y) {
    uint16_t pos = y * SCREEN_WIDTH + x;

    outb(0x3D4, 0x0F);
    outb(0x3D5, (uint8_t)(pos & 0xFF));
    outb(0x3D4, 0x0E);
    outb(0x3D5, (uint8_t)((pos >> 8) & 0xFF));
}

void clrscreen() {
    for (int y = 0; y < SCREEN_HEIGHT; y++) {
        for (int x = 0; x < SCREEN_WIDTH; x++) {
            set_screen_chr(x, y, 0);
            set_screen_color(x, y, DEFAULT_COLOR);
        }
    }

    pos_x = 0;
    pos_y = 0;
    move_curs(pos_x, pos_y);
}

void scroll_up(int lines) {
    for (int y = 0; y < SCREEN_HEIGHT; y++) {
        for (int x = 0; x < SCREEN_WIDTH; x++) {
            set_screen_chr(x, y - lines, get_screen_chr(x, y));
            set_screen_color(x, y - lines, get_screen_color(x, y));
        }
    }
    for (int y = SCREEN_HEIGHT - lines; y < SCREEN_HEIGHT; y++) {
        for (int x = 0; x < SCREEN_WIDTH; x++)
        {
            set_screen_chr(x, y, '\0');
            set_screen_color(x, y, DEFAULT_COLOR);
        }
    }
    pos_y -= lines;
}

void putc(char c) {
    switch (c) {
        case '\n':
            pos_x = 0;
            pos_y++;
            break;
        case '\r':
            pos_x = 0;
            break;
        case '\t':
            for (int i = 0; i < 4 - (pos_x % 4); i++) putc(' ');
            break;
        default:
            set_screen_chr(pos_x, pos_y, c);
            pos_x++;
            break;
    }

    if (pos_x >= SCREEN_WIDTH) {
        pos_x = 0;
        pos_y++;
    }
    if (pos_y >= SCREEN_HEIGHT) {
        scroll_up(1);
    }

    move_curs(pos_x, pos_y);
}

void puts(const char* s) {
    for (; *s; s++) {
        putc(*s);
    }
}

const char HEX_CHARS[]       = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'a', 'b', 'c', 'd', 'e', 'f'};

void print_uint(unsigned long long number, int base) {
    char buffer[32];
    int pos = 0;

    do {
        unsigned long long rem = number % base;
        number /= base;
        buffer[pos++] = HEX_CHARS[rem];
    } while (number > 0);

    while (--pos >= 0) {
        putc(buffer[pos]);
    }
}

void print_int(long long number, int base) {
    if (number < 0) {
        putc('-');
        print_uint(-number, base);
    } else {
        print_uint(number, base);
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

void printf(const char* fmt, ...) {
    va_list args;
    va_start(args, fmt);

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
                    putc(*fmt);
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
                        putc((char)va_arg(args, int));
                        break;
                    case 's':
                        puts(va_arg(args, const char*));
                        break;
                    case '%':
                        putc('%');
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
                                print_int(va_arg(args, int), base);
                                break;
                            case LENGTH_L:
                                print_int(va_arg(args, long), base);
                                break;
                            case LENGTH_LL:
                                print_int(va_arg(args, long long), base);
                                break;
                        }
                    } else {
                        switch (length) {
                            case LENGTH_HH:
                            case LENGTH_H:
                            case LENGTH_DEF:
                                print_uint(va_arg(args, unsigned int), base);
                                break;
                            case LENGTH_L:
                                print_uint(va_arg(args, unsigned long), base);
                                break;
                            case LENGTH_LL:
                                print_uint(va_arg(args, unsigned long long), base);
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

    va_end(args);
}
