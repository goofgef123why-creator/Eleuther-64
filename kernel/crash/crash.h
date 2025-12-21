#ifndef CRASH_H
#define CRASH_H
__attribute__((noreturn))
void _crash();
void _panic(const uint64_t error_code[], uint16_t count);
#endif