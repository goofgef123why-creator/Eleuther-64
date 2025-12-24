#ifndef SERIAL_H
#define SERIAL_H
#include <stdint.h>
#define COM1 0x03F8
static inline void _outb(uint16_t port, uint8_t val) {
    asm volatile ("outb %0, %1" :: "a"(val), "d"(port));
}
static inline uint8_t _inb(uint16_t port) {
    uint8_t ret;
    asm volatile ("inb %1, %0" : "=a"(ret) : "d"(port));
    return ret;
}
void _initcom1(void);
int _transmit0(void);
void _serialchar(char c);
void _serialwrite(const char* str);
#endif
