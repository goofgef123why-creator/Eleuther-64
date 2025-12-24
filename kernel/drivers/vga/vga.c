#include <stdint.h>
#include "drivers/vga.h"
#include "drivers/serial.h"
#define WIDTH 80
#define HEIGHT 25
static int row = 0;
static int col = 0;
static uint8_t current_color = 0x0F;
static volatile uint16_t* const vga = (volatile uint16_t*)0xB8000;
static uint8_t flags[WIDTH * HEIGHT];
uint16_t VGACELL(char c, uint8_t color) {
    return (uint16_t)c | ((uint16_t)color << 8);
}
static inline void _vgaupdatecursor(void) {
    uint16_t pos = row * WIDTH + col;
    _outb(0x3D4, 0x0F);
    _outb(0x3D5, (uint8_t)(pos & 0xFF));
    _outb(0x3D4, 0x0E);
    _outb(0x3D5, (uint8_t)((pos >> 8) & 0xFF));
}
void _vgabackspace(void) {
    if (row == 0 && col == 0) {
        return;
    }
    int prev_row = row;
    int prev_col = col;
    if (prev_col > 0) {
        prev_col--;
    } else {
        prev_row--;
        prev_col = WIDTH - 1;
    }
    if (flags[prev_row * WIDTH + prev_col] == 1) {
        return;
    }
    row = prev_row;
    col = prev_col;
    vga[row * WIDTH + col] = VGACELL(' ', current_color);
    flags[row * WIDTH + col] = 0;
    _vgaupdatecursor();
}
static void _scroll(void) {
    for (int y = 1; y < HEIGHT; y++) {
        for (int x = 0; x < WIDTH; x++) {
            vga[(y - 1) * WIDTH + x] = vga[y * WIDTH + x];
            flags[(y - 1) * WIDTH + x] = flags[y * WIDTH + x];
        }
    }
    for (int x = 0; x < WIDTH; x++) {
        vga[(HEIGHT - 1) * WIDTH + x] = VGACELL(' ', current_color);
        flags[(HEIGHT - 1) * WIDTH + x] = 0;
    }
    row = HEIGHT - 1;
    col = 0;
    _vgaupdatecursor();
}
void _clean(void) {
    for (int i = 0; i < WIDTH * HEIGHT; i++) {
        vga[i] = VGACELL(' ', current_color);
        flags[i] = 0;
    }
    row = 0;
    col = 0;
}
static void _vgachar(char c) {
    if (c == '\n') {
        col = 0;
        row++;
        if (row >= HEIGHT) {
            _scroll();
        }
        _vgaupdatecursor();
        return;
    }
    int idx = row * WIDTH + col;
    vga[idx] = VGACELL(c, current_color);
    flags[idx] = 0;
    if (++col >= WIDTH) {
        col = 0;
        row++;
        if (row >= HEIGHT) {
            _scroll();
        }
    }
    _vgaupdatecursor();
}
void _avgachar(int x, int y,char c, uint8_t color){
    if (x < 0 || x >= WIDTH || y < 0 || y >= HEIGHT){
        return;
    }
    int idx = y * WIDTH + x;
    vga[idx] = VGACELL(c,color);
    flags[idx] = 0;
}
static void _svgachar(char c) {
    if (c == '\n') {
        col = 0;
        row++;
        if (row >= HEIGHT) {
            _scroll();
        }
        return;
    }
    int idx = row * WIDTH + col;
    vga[idx] = VGACELL(c, current_color);
    flags[idx] = 1;
    if (++col >= WIDTH) {
        col = 0;
        row++;
        if (row >= HEIGHT) {
            _scroll();
        }
    }
}
void _vgawrite(const char* buf, uint8_t color) {
    current_color = color;
    for (unsigned int i = 0; buf[i]; i++) {
        _vgachar(buf[i]);
    }
}
void _svgawrite(const char* buf, uint8_t color){
    current_color = color;
    for (unsigned int i = 0; buf[i]; i++) {
        _svgachar(buf[i]);
    }
}