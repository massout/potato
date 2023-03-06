#ifndef FORMAT_H
#define FORMAT_H

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
int atoi(const char* p) {
    int x = 0;
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

/**
 * We start at the end of the string, and work our way backwards, inserting the digits of the number
 * as we go
 *
 * @param i The number to be converted to a string
 * @param b The buffer to store the string in.
 *
 * @return The address of the first element of the array.
 */
char* itoa(int i, char b[]) {
    char const digit[] = "0123456789";
    char* p = b;

    if (i < 0) {
        *p++ = '-';
        i *= -1;
    }

    int shifter = i;

    do {  // Move to where representation ends
        ++p;
        shifter = shifter / 10;
    } while (shifter);

    *p = '\0';

    do {  // Move back, inserting digits as u go
        *--p = digit[i % 10];
        i = i / 10;
    } while (i);

    return b;
}

void printf(int x, int y, char* args, ...) {
}

#endif