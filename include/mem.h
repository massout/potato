#ifndef MEM_H
#define MEM_H

#include "intdefs.h"

void memcpy(void* src, void* dst, uint64_t n) {
    uint8_t* a = src;
    uint8_t* b = dst;

    while (n-- > 0) {
        (*b) = (*a);

        a++;
        b++;
    }
}

int memcmp(void* s1, void* s2, uint64_t n) {
    uint8_t* a = s1;
    uint8_t* b = s2;

    while (n-- > 0) {
        if (*a != *b)
            return (*a) - (*b);

        a++;
        b++;
    }

    return 0;
}

void memset(void* dest, char c, uint64_t n) {
    uint8_t* a = dest;

    while (n-- > 0) {
        (*a) = c;

        a++;
    }
}

#endif