#ifndef TIMER_H
#define TIMER_H

#include "intdefs.h"
#include "io.h"

#define SYSTMR_LO ((volatile uint32_t *)(MMIO_BASE + 0x00003004))
#define SYSTMR_HI ((volatile uint32_t *)(MMIO_BASE + 0x00003008))

/**
 * It does nothing
 *
 * @param n number of cycles to sleep
 */
void sleep_cycles(uint32_t n) {
    if (n)
        while (n--)
            asm volatile("nop");
}

/**
 * It sleeps for n milliseconds
 *
 * @param n The number of milliseconds to sleep.
 */
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

/**
 * It sleeps for n seconds
 *
 * @param n The number of seconds to sleep
 */
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

/**
 * It reads the system timer twice, and if the high word changes between the two reads, it reads it
 * again
 *
 * @return The current system timer value.
 */
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

/**
 * "If the system timer is running, wait until it has counted up by n milliseconds."
 *
 * The first line of the function is a comment. Comments are ignored by the compiler. They are used to
 * document the code
 *
 * @param n The number of milliseconds to sleep.
 */
void sleep_msec_st(uint32_t n) {
    uint64_t t = get_system_timer();

    if (t)
        while (get_system_timer() < t + n)
            ;
}

#endif