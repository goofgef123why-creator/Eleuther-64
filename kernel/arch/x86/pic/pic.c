#include "arch/pic.h"
#include "drivers/serial.h"
#define PIC1_COMMAND 0x20
#define PIC1_DATA    0x21
#define PIC2_COMMAND 0xA0
#define PIC2_DATA    0xA1
#define ICW1_INIT 0x10
#define ICW1_ICW4 0x01
#define ICW4_8086 0x01
void _picremap(void) {
    uint8_t a1 = _inb(PIC1_DATA);
    uint8_t a2 = _inb(PIC2_DATA);
    _outb(PIC1_COMMAND, ICW1_INIT | ICW1_ICW4);
    _outb(PIC2_COMMAND, ICW1_INIT | ICW1_ICW4);
    _outb(PIC1_DATA, 0x20);
    _outb(PIC2_DATA, 0x28);
    _outb(PIC1_DATA, 4);
    _outb(PIC2_DATA, 2);
    _outb(PIC1_DATA, ICW4_8086);
    _outb(PIC2_DATA, ICW4_8086);
    _outb(PIC1_DATA, a1);
    _outb(PIC2_DATA, a2);
}
void _picenableirq(uint8_t irq) {
    uint16_t port = (irq < 8) ? PIC1_DATA : PIC2_DATA;
    uint8_t value = _inb(port);
    value &= ~(1 << (irq % 8));
    _outb(port, value);
}