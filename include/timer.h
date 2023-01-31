#ifndef TIMER_H
#define TIMER_H

#include "gpio.h"
#include "intdefs.h"

#define SYSTMR_LO ((volatile uint32_t *)(MMIO_BASE + 0x00003004))
#define SYSTMR_HI ((volatile uint32_t *)(MMIO_BASE + 0x00003008))

void sleep_cycles(uint32_t n) {
    if (n)
        while (n--)
            asm volatile("nop");
}

void sleep_msec(uint32_t n) {
    register uint64_t f, t, r;

    asm volatile("mrs %0, cntfrq_el0"
                 : "=r"(f));

    asm volatile("mrs %0, cntpct_el0"
                 : "=r"(t));

    t += ((f / 1000) * n) / 1000;

    do {
        asm volatile("mrs %0, cntpct_el0"
                     : "=r"(r));
    } while (r < t);
}

void sleep_sec(uint32_t n) {
    register uint64_t f, t, r;

    asm volatile("mrs %0, cntfrq_el0"
                 : "=r"(f));

    asm volatile("mrs %0, cntpct_el0"
                 : "=r"(t));

    t += ((f / 1000) * n) * 1000;

    do {
        asm volatile("mrs %0, cntpct_el0"
                     : "=r"(r));
    } while (r < t);
}

uint64_t get_system_timer() {
    uint32_t h = -1, l;

    h = *SYSTMR_HI;
    l = *SYSTMR_LO;

    if (h != *SYSTMR_HI) {
        h = *SYSTMR_HI;
        l = *SYSTMR_LO;
    }

    return ((uint64_t)h << 32) | l;
}

void sleep_msec_st(uint32_t n) {
    uint64_t t = get_system_timer();

    if (t)
        while (get_system_timer() < t + n)
            ;
}

#endif