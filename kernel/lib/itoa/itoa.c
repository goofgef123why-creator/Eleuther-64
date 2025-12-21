#include <stdint.h>
#include "lib/itoa.h"
int isnum(const char *s) {
    if (!s || *s == '\0')
        return 0;

    for (; *s; s++) {
        if (*s < '0' || *s > '9')
            return 0;
    }

    return 1;
}
void itoa(uint64_t value, char *buf, int base) {
    char *p = buf;
    char *start;
    if (base == 16) {
        *p++ = '0';
        *p++ = 'x';
    }
    start = p;
    if (value == 0) {
        *p++ = '0';
        *p = '\0';
        return;
    }
    while (value > 0) {
        uint64_t digit = value % base;
        *p++ = (digit < 10) ? ('0' + digit) : ('A' + digit - 10);
        value /= base;
    }
    *p = '\0';
    p--;
    while (start < p) {
        char tmp = *start;
        *start++ = *p;
        *p-- = tmp;
    }
}
