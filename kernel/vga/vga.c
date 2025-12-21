#include <stdint.h>
#include "vga.h"
#define WIDTH 80
#define HEIGHT 25
static int row = 0;
static int col = 0;
static uint8_t current_color = 0x0F;
static volatile uint16_t* const vga = (volatile uint16_t*)0xB8000;
static inline uint16_t VGACELL(char c, uint8_t color) {
    return (uint16_t)c | ((uint16_t)color << 8);
}
static void _scroll(void) {
    for (int y = 1; y < HEIGHT; y++) {
        for (int x = 0; x < WIDTH; x++) {
            vga[(y - 1) * WIDTH + x] =
                vga[y * WIDTH + x];
        }
    }

    for (int x = 0; x < WIDTH; x++) {
        vga[(HEIGHT - 1) * WIDTH + x] = VGACELL(' ', current_color);
    }

    row = HEIGHT - 1;
    col = 0;
}
void _clean(void) {
    for (int i = 0; i < WIDTH * HEIGHT; i++) {
        vga[i] = VGACELL(' ', current_color);
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
        return;
    }
    vga[row * WIDTH + col] = VGACELL(c, current_color);

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