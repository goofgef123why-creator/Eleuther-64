#include <stdint.h>
#include "drivers/serial.h"
#define COM1 0x03F8
static inline void _outb(uint16_t port, uint8_t val);
static inline uint8_t _inb(uint16_t port);
void _initcom1() {
    // why
    _outb(COM1 + 1, 0x00); 
    _outb(COM1 + 3, 0x80);    
    _outb(COM1 + 0, 0x0C);
    _outb(COM1 + 1, 0x00);
    _outb(COM1 + 3, 0x03);    
    _outb(COM1 + 2, 0xC7);    
    _outb(COM1 + 4, 0x0B);   
}
int _transmit0(){
    return _inb(COM1 + 5) & 0x20;
}
void _serialchar(char c){
    while (!_transmit0());
    _outb(COM1, c);
}
void _serialwrite(const char* str){
    for (int i = 0; str[i] != '\0'; i++){
        _serialchar(str[i]);
    }
}