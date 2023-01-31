#ifndef MEM_H
#define MEM_H

#include "intdefs.h"

/**
 * It copies n bytes from src to dst
 *
 * @param src The source address to copy from.
 * @param dst The destination address to copy to.
 * @param n The number of bytes to copy.
 */
void memcpy(void* src, void* dst, uint64_t n) {
    uint8_t* a = src;
    uint8_t* b = dst;

    while (n-- > 0) {
        (*b) = (*a);

        a++;
        b++;
    }
}

/**
 * It compares two memory regions and returns the difference between the first two bytes that differ
 *
 * @param s1 The first string to compare.
 * @param s2 The string to compare to.
 * @param n The number of bytes to compare.
 *
 * @return The difference between the first two different bytes.
 */
void memcmp(void* s1, void* s2, uint64_t n) {
    uint8_t* a = s1;
    uint8_t* b = s2;

    while (n-- > 0) {
        if (*a != *b)
            return (*a) - (*b);

        a++;
        b++;
    }
}

/**
 * It takes a pointer to a memory location, a character, and a number of bytes, and it sets the memory
 * location to the character for the number of bytes
 *
 * @param dest The destination address to write to.
 * @param c The character to fill the memory with.
 * @param n The number of bytes to copy.
 */
void memset(void* dest, char c, uint64_t n) {
    uint8_t* a = dest;

    while (n-- > 0) {
        (*a) = c;

        a++;
    }
}

#endif