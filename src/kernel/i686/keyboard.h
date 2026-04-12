#ifndef _KEYBOARD_H_
#define _KEYBOARD_H_

#include <stdbool.h>

void initialize_keyboard();

void set_typing_enabled_keyboard(bool enabled);
bool is_typing_enabled_keyboard();

#endif