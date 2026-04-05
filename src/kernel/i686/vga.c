#include "vga.h"
#include "ports.h"
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

void move_curs_VGA(int x, int y) {
    uint16_t pos = y * SCREEN_WIDTH + x;

    outb(0x3D4, 0x0F);
    outb(0x3D5, (uint8_t)(pos & 0xFF));
    outb(0x3D4, 0x0E);
    outb(0x3D5, (uint8_t)((pos >> 8) & 0xFF));
}

void clrscreen_VGA() {
    for (int y = 0; y < SCREEN_HEIGHT; y++) {
        for (int x = 0; x < SCREEN_WIDTH; x++) {
            set_screen_chr(x, y, 0);
            set_screen_color(x, y, DEFAULT_COLOR);
        }
    }

    pos_x = 0;
    pos_y = 0;
    move_curs_VGA(pos_x, pos_y);
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

void putc_VGA(char c) {
    switch (c) {
        case '\n':
            pos_x = 0;
            pos_y++;
            break;
        case '\r':
            pos_x = 0;
            break;
        case '\t':
            for (int i = 0; i < 4 - (pos_x % 4); i++) putc_VGA(' ');
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

    move_curs_VGA(pos_x, pos_y);
}