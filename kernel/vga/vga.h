#ifndef VGA_H
#define VGA_H
static inline uint16_t VGACELL(char c, uint8_t color);
static void _scroll(void);
void _clean(void);
static void _putchar(char c);
void _vgawrite(const char* buf, uint8_t color);
#endif