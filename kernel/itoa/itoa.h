#ifndef ITOA
#define ITOA
#include <stdint.h>
int isnum(const char *s);
void itoa(uint64_t value, char *buf, int base);
#endif