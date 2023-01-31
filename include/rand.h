#ifndef RAND_H
#define RAND_H

#include "intdefs.h"
#include "io.h"

#define RNG_BASE ((volatile uint32_t *)(MMIO_BASE + 0x00104000))

#define RNG_CTRL ((volatile uint32_t *)(RNG_BASE + 0x00))
#define RNG_STATUS ((volatile uint32_t *)(RNG_BASE + 0x04))
#define RNG_DATA ((volatile uint32_t *)(RNG_BASE + 0x08))
#define RNG_INT_MASK ((volatile uint32_t *)(RNG_BASE + 0x10))

/**
 * It initializes the random number generator
 */
void rand_init() {
    *RNG_STATUS = 0x40000;
    *RNG_INT_MASK |= 1;
    *RNG_CTRL |= 1;
}

/**
 * It waits for the random number generator to be ready, then returns a random number between min and
 * max
 *
 * @param min The minimum value to return.
 * @param max The maximum value that can be returned.
 *
 * @return A random number between min and max.
 */
uint32_t rand(uint32_t min, uint32_t max) {
    while (!((*RNG_STATUS) >> 24))
        asm volatile("nop");

    return *RNG_DATA % (max - min) + min;
}

#endif