#ifndef CRASH_H
#define CRASH_H
__attribute__((noreturn))
void _crash();
void _panic(const char* error_codes);
#endif