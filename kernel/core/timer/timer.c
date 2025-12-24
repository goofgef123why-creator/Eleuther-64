#include "core/timer.h"
#include "drivers/serial.h"
#include "lib/itoa.h"
#define PIT_CMD   0x43
#define PIT_CH0   0x40
#define PIT_FREQ  1193182
volatile uint64_t timer_ticks = 0;
void _inittimer(uint32_t hz) {
    uint32_t divisor = PIT_FREQ / hz;
    _outb(PIT_CMD, 0x36);
    _outb(PIT_CH0, divisor & 0xFF);
    _outb(PIT_CH0, (divisor >> 8) & 0xFF);
}
void _timesleep(uint64_t ticks) {
    uint64_t target = timer_ticks + ticks;
    while (timer_ticks < target) {
        asm volatile("hlt");
    }
}
void _sleepms(uint64_t ms) {
    _timesleep((ms * 100) / 1000);
}