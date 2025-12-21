#include <stdint.h>
#include "drivers/serial.h"
#define COM1 0x03F8
static inline void outb(uint16_t port, uint8_t val);
static inline uint8_t inb(uint16_t port);
void _initcom1() {
    // why
    outb(COM1 + 1, 0x00); 
    outb(COM1 + 3, 0x80);    
    outb(COM1 + 0, 0x0C);
    outb(COM1 + 1, 0x00);
    outb(COM1 + 3, 0x03);    
    outb(COM1 + 2, 0xC7);    
    outb(COM1 + 4, 0x0B);   
}
int _transmit0(){
    return inb(COM1 + 5) & 0x20;
}
void _serialchar(char c){
    while (!_transmit0());
    outb(COM1, c);
}
void _serialwrite(const char* str){
    for (int i = 0; str[i] != '\0'; i++){
        _serialchar(str[i]);
    }
}