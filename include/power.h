#ifndef POWER_H
#define POWER_H

#include "gpio.h"
#include "intdefs.h"
#include "mbox.h"
#include "timer.h"

#define PM_RSTC ((volatile uint32_t *)(MMIO_BASE + 0x0010001c))
#define PM_RSTS ((volatile uint32_t *)(MMIO_BASE + 0x00100020))
#define PM_WDOG ((volatile uint32_t *)(MMIO_BASE + 0x00100024))

#define PM_WDOG_MAGIC 0x5a000000
#define PM_RSTC_FULLRST 0x00000020

void power_off() {
    uint64_t r;

    for (r = 0; r < 16; r++) {
        mbox[0] = 8 * 4;
        mbox[1] = MBOX_REQUEST;
        mbox[2] = MBOX_TAG_SETPOWER;
        mbox[3] = 8;
        mbox[4] = 8;
        mbox[5] = (uint32_t)r;
        mbox[6] = 0;
        mbox[7] = MBOX_TAG_LAST;

        mbox_call(MBOX_CH_PROP);
    }

    *GPFSEL0 = 0;
    *GPFSEL1 = 0;
    *GPFSEL2 = 0;
    *GPFSEL3 = 0;
    *GPFSEL4 = 0;
    *GPFSEL5 = 0;
    *GPPUD = 0;

    sleep_cycles(150);

    *GPPUDCLK0 = 0xffffffff;
    *GPPUDCLK1 = 0xffffffff;

    sleep_cycles(150);

    *GPPUDCLK0 = 0;
    *GPPUDCLK1 = 0;

    r = *PM_RSTS;
    r &= ~0xfffffaaa;
    r |= 0x555;

    *PM_RSTS = PM_WDOG_MAGIC | r;
    *PM_WDOG = PM_WDOG_MAGIC | 10;
    *PM_RSTC = PM_WDOG_MAGIC | PM_RSTC_FULLRST;
}

void reboot() {
    uint32_t r;

    r = *PM_RSTS;
    r &= ~0xfffffaaa;

    *PM_RSTS = PM_WDOG_MAGIC | r;
    *PM_WDOG = PM_WDOG_MAGIC | 10;
    *PM_RSTC = PM_WDOG_MAGIC | PM_RSTC_FULLRST;
}

#endif