#ifndef VGA_H
#define VGA_H
#include <stdint.h>
uint16_t VGACELL(char c, uint8_t color);
void _vgabackspace(void);
static void _scroll(void);
void _clean(void);
static void _putchar(char c);
static void _sputchar(char c);
void _avgachar(int x, int y,char c, uint8_t color);
void _vgawrite(const char* buf, uint8_t color);
void _svgawrite(const char* buf, uint8_t color);
#endif