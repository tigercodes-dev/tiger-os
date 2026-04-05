#ifndef _VGA_H_
#define _VGA_H_

#include <stdint.h>

void clrscreen_VGA();
void putc_VGA(char c);
void move_curs_VGA(int x, int y);

#endif