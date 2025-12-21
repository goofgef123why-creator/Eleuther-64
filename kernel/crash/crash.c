#include <stdint.h>
#include "crash.h"
#include "itoa/itoa.h"
#define WIDTH 80
#define HEIGHT 25
void _crash(){
    for (;;) {
        __asm__ __volatile__ (
            "cli\n\t"
            "hlt"
        );
    }
}
static int row = 0;
static int col = 0;
static uint8_t current_color = 0x4F;
static volatile uint16_t* const vga = (volatile uint16_t*)0xB8000;
static inline uint16_t VGACELL_P(char c, uint8_t color) {
    return (uint16_t)c | ((uint16_t)color << 8);
}
static void SCROLL_P(void) {
    for (int y = 1; y < HEIGHT; y++) {
        for (int x = 0; x < WIDTH; x++) {
            vga[(y - 1) * WIDTH + x] =
                vga[y * WIDTH + x];
        }
    }
    for (int x = 0; x < WIDTH; x++) {
        vga[(HEIGHT - 1) * WIDTH + x] = VGACELL_P(' ', current_color);
    }
    row = HEIGHT - 1;
    col = 0;
}
void CLEAR_P(void) {
    for (int i = 0; i < WIDTH * HEIGHT; i++) {
        vga[i] = VGACELL_P(' ', current_color);
    }
    row = 0;
    col = 0;
}
static void PUTCHAR_P(char c) {
    if (c == '\n') {
        col = 0;
        row++;
        if (row >= HEIGHT) {
            SCROLL_P();
        }
        return;
    }
    vga[row * WIDTH + col] = VGACELL_P(c, current_color);

    if (++col >= WIDTH) {
        col = 0;
        row++;
        if (row >= HEIGHT) {
            SCROLL_P();
        }
    }
}
void VGAWRITE_P(const char* buf, uint8_t color) {
    current_color = color;
    for (unsigned int i = 0; buf[i]; i++) {
        PUTCHAR_P(buf[i]);
    }
}
const char* CODECONVER(uint64_t code){
    switch (code)
    {
    case 0x2F1:
        return "Test Error: Testing errors, no likely faults.";
        break;
    case 0x2F2:
        return "Unknow Error: We are unsure of the problem.";
        break;
    case 0x2F3:
        return "Memory: We have ran out of memory";
        break;
    case 0x2F4:
        return "Memory: Not enough memory to fully boot.";
        break;
    case 0x2F5:
        return "Memory: RAM not found.";
        break;
    default:
        break;
    }
}
void _panic(const uint64_t error_codes[], uint16_t count){
    CLEAR_P();
    VGAWRITE_P("Oops!\n",0x4F);
    VGAWRITE_P("We must of have jammed into an Error, please standby.\n\n",0x4F);
    VGAWRITE_P("ERROR: \n", 0x4F);
    for (int x = 0; x < count; x++){
        VGAWRITE_P(CODECONVER(error_codes[x]), 0x4F);
        VGAWRITE_P("\n",0x4F);
    }
    _crash();
}