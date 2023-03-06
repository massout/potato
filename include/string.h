#ifndef STRING_H
#define STRING_H

#include "intdefs.h"

/**
 * It converts a string to an integer
 *
 * Taken from http://tinodidriksen.com/uploads/code/cpp/speed-string-to-int.cpp
 *
 * @param p pointer to the string to convert
 *
 * @return the integer value of the string.
 */
uint64_t str2int(const char *p) {
    uint64_t x = 0;
    uint8_t neg = 0;

    if (*p == '-') {
        neg = 1;
        ++p;
    }

    while (*p >= '0' && *p <= '9') {
        x = (x * 10) + (*p - '0');
        ++p;
    }

    if (neg) {
        x = -x;
    }

    return x;
}

#endif