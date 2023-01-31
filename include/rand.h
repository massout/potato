#ifndef RAND_H
#define RAND_H

#include "gpio.h"
#include "intdefs.h"

#define RNG_BASE ((volatile uint32_t *)(MMIO_BASE + 0x00104000))

#define RNG_CTRL ((volatile uint32_t *)(RNG_BASE + 0x00))
#define RNG_STATUS ((volatile uint32_t *)(RNG_BASE + 0x04))
#define RNG_DATA ((volatile uint32_t *)(RNG_BASE + 0x08))
#define RNG_INT_MASK ((volatile uint32_t *)(RNG_BASE + 0x10))

void rand_init() {
    *RNG_STATUS = 0x40000;
    *RNG_INT_MASK |= 1;
    *RNG_CTRL |= 1;
}

uint32_t rand(uint32_t min, uint32_t max) {
    while (!((*RNG_STATUS) >> 24))
        asm volatile("nop");

    return *RNG_DATA % (max - min) + min;
}

#endif