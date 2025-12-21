#include "idt.h"
#include "serial/serial.h"
#include "crash/crash.h"
#include "itoa/itoa.h"
#include <stdint.h>
static idt_entry_t idt[256];
static idt_ptr_t idt_ptr;
extern void isr0(void);
extern void isr1(void);
extern void isr2(void);
extern void isr3(void);
extern void isr4(void);
extern void isr5(void);
extern void isr6(void);
extern void isr7(void);
extern void isr8(void);
extern void isr9(void);
extern void isr10(void);
extern void isr11(void);
extern void isr12(void);
extern void isr13(void);
extern void isr14(void);
extern void isr15(void);
extern void isr16(void);
extern void isr17(void);
extern void isr18(void);
extern void isr19(void);
extern void isr20(void);
extern void isr21(void);
extern void isr22(void);
extern void isr23(void);
extern void isr24(void);
extern void isr25(void);
extern void isr26(void);
extern void isr27(void);
extern void isr28(void);
extern void isr29(void);
extern void isr30(void);
extern void isr31(void);
extern void irq0(void);
extern void irq1(void);
extern void irq2(void);
extern void irq3(void);
extern void irq4(void);
extern void irq5(void);
extern void irq6(void);
extern void irq7(void);
extern void irq8(void);
extern void irq9(void);
extern void irq10(void);
extern void irq11(void);
extern void irq12(void);
extern void irq13(void);
extern void irq14(void);
extern void irq15(void);
static void idt_set_gate(uint8_t n, uint64_t handler) {
    idt[n].offset_low  = handler & 0xFFFF;
    idt[n].selector    = KERNEL_CS;
    idt[n].ist         = 0;
    idt[n].type_attr   = 0x8E;
    idt[n].offset_mid  = (handler >> 16) & 0xFFFF;
    idt[n].offset_high = (handler >> 32) & 0xFFFFFFFF;
    idt[n].zero        = 0;
}
void _isrhandler(regs_t *r){
    if (r->int_no == 14) {
        uint64_t cr2;
        __asm__ volatile ("mov %%cr2, %0" : "=r"(cr2));
        _serialwrite("PAGE FAULT\n");
        _serialwrite("ADDR: ");
        _serialwrite("\nERR: ");
        _serialwrite("\n");
    }else {
        _serialwrite("CPU EXCEPTION: ");
        _serialwrite("\n");
    }
    _crash();
}
extern volatile uint64_t timer_ticks;
void _irqhandler(regs_t *r) {
    if (r->int_no == 32) {
        timer_ticks++;
    }
    if (r->int_no >= 40){
        outb(0xA0, 0x20);
    }
    outb(0x20, 0x20);
}
void _initidt(void) {
    idt_ptr.limit = sizeof(idt) - 1;
    idt_ptr.base  = (uint64_t)&idt;
    for (int i = 0; i < 256; i++){
        idt_set_gate(i, 0);
    }
    idt_set_gate(0,  (uint64_t)isr0);
    idt_set_gate(1,  (uint64_t)isr1);
    idt_set_gate(2,  (uint64_t)isr2);
    idt_set_gate(3,  (uint64_t)isr3);
    idt_set_gate(4,  (uint64_t)isr4);
    idt_set_gate(5,  (uint64_t)isr5);
    idt_set_gate(6,  (uint64_t)isr6);
    idt_set_gate(7,  (uint64_t)isr7);
    idt_set_gate(8,  (uint64_t)isr8);
    idt_set_gate(9,  (uint64_t)isr9);
    idt_set_gate(10, (uint64_t)isr10);
    idt_set_gate(11, (uint64_t)isr11);
    idt_set_gate(12, (uint64_t)isr12);
    idt_set_gate(13, (uint64_t)isr13);
    idt_set_gate(14, (uint64_t)isr14);
    idt_set_gate(15, (uint64_t)isr15);
    idt_set_gate(16, (uint64_t)isr16);
    idt_set_gate(17, (uint64_t)isr17);
    idt_set_gate(18, (uint64_t)isr18);
    idt_set_gate(19, (uint64_t)isr19);
    idt_set_gate(20, (uint64_t)isr20);
    idt_set_gate(21, (uint64_t)isr21);
    idt_set_gate(22, (uint64_t)isr22);
    idt_set_gate(23, (uint64_t)isr23);
    idt_set_gate(24, (uint64_t)isr24);
    idt_set_gate(25, (uint64_t)isr25);
    idt_set_gate(26, (uint64_t)isr26);
    idt_set_gate(27, (uint64_t)isr27);
    idt_set_gate(28, (uint64_t)isr28);
    idt_set_gate(29, (uint64_t)isr29);
    idt_set_gate(30, (uint64_t)isr30);
    idt_set_gate(31, (uint64_t)isr31);
    idt_set_gate(32, (uint64_t)irq0);
    idt_set_gate(33, (uint64_t)irq1);
    idt_set_gate(34, (uint64_t)irq2);
    idt_set_gate(35, (uint64_t)irq3);
    idt_set_gate(36, (uint64_t)irq4);
    idt_set_gate(37, (uint64_t)irq5);
    idt_set_gate(38, (uint64_t)irq6);
    idt_set_gate(39, (uint64_t)irq7);
    idt_set_gate(40, (uint64_t)irq8);
    idt_set_gate(41, (uint64_t)irq9);
    idt_set_gate(42, (uint64_t)irq10);
    idt_set_gate(43, (uint64_t)irq11);
    idt_set_gate(44, (uint64_t)irq12);
    idt_set_gate(45, (uint64_t)irq13);
    idt_set_gate(46, (uint64_t)irq14);
    idt_set_gate(47, (uint64_t)irq15);
    asm volatile ("lidt %0" : : "m"(idt_ptr));
    asm volatile ("sti");
}