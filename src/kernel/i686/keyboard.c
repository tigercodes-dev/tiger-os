#include "keyboard.h"
#include "irq.h"
#include "ports.h"
#include "../log.h"
#include "../stdio.h"
#include "../ctype.h"

const char scancodes[] = {
    0x00, '\e', '1', '2',
    '3', '4', '5', '6',
    '7', '8', '9', '0',
    '-', '=', '\b', '\t',
    'q', 'w', 'e', 'r',
    't', 'y', 'u', 'i',
    'o', 'p', '[', ']',
    '\n', 0xF2, 'a', 's',
    'd', 'f', 'g', 'h',
    'j', 'k', 'l', ';',
    '\'', '`', 0xF0, '\\',
    'z', 'x', 'c', 'v',
    'b', 'n', 'm', ',',
    '.', '/', 0xF1, '*',
    0xF4, ' ', 0xD0, 0xC1,
    0xC2, 0xC3, 0xC4, 0xC5,
    0xC6, 0xC7, 0xC8, 0xC9,
    0xCA, 0xD1, 0xD2, '7',
    '8', '9', '-', '4',
    '5', '6', '+', '1',
    '2', '3', '0', '.',
    0x00, 0x00, 0x00, 0xCB,
    0xCC, 0x00, 0x00, 0x00,
};

static bool shift_pressed = false;
static bool extended = false;
static bool typing_enabled = false;

char shift_chr(char c) {
    if (islower(c)) {
        return toupper(c);
    }

    switch (c) {
        case '`': return '~';
        case '1': return '!';
        case '2': return '@';
        case '3': return '#';
        case '4': return '$';
        case '5': return '%';
        case '6': return '^';
        case '7': return '&';
        case '8': return '*';
        case '9': return '(';
        case '0': return ')';
        case '-': return '_';
        case '=': return '+';
        case '[': return '{';
        case ']': return '}';
        case '\\': return '|';
        case ';': return ':';
        case '\'': return '"';
        case ',': return '<';
        case '.': return '>';
        case '/': return '?';
        default: return c;
    }
}

void set_typing_enabled_keyboard(bool enabled) {
    typing_enabled = enabled;
}

bool is_typing_enabled_keyboard() {
    return typing_enabled;
}

void keypress(InterruptStack* stack) {
    uint8_t scancode = inb(0x60);

    if (extended) {
        if (scancode < 0x80) {
            logf(DEBUG, "Extended scancode: E0 %x", scancode);
        }
        extended = false;
    } else {
        if (scancode == 0x2A || scancode == 0x36) {
            shift_pressed = true;
        } else if (scancode == 0xAA || scancode == 0xB6) {
            shift_pressed = false;
        } else if (scancode == 0xE0) {
            extended = true;
        } else {
            unsigned char c = scancodes[scancode < 0x60 ? scancode : 0x00];
            if (typing_enabled && (isprint(c) || c == '\t' || c == '\n' || c == '\b')) putc(shift_pressed ? shift_chr(c) : c);
        }
    }
}

void initialize_keyboard() {
    register_handler_IRQ(1, keypress);
}