#pragma once
#include <stdint.h>
void _inittimer(uint32_t hz);
extern volatile uint64_t timer_ticks;
void _timesleep(uint64_t ticks);
void _sleepms(uint64_t ms);