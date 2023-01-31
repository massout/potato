#ifndef MBOX_H
#define MBOX_H

#include "gpio.h"
#include "intdefs.h"

#define MBOX_REQUEST 0

#define MBOX_CH_POWER 0
#define MBOX_CH_FB 1
#define MBOX_CH_VUART 2
#define MBOX_CH_VCHIQ 3
#define MBOX_CH_LEDS 4
#define MBOX_CH_BTNS 5
#define MBOX_CH_TOUCH 6
#define MBOX_CH_COUNT 7
#define MBOX_CH_PROP 8

#define MBOX_TAG_SETPOWER 0x28001
#define MBOX_TAG_SETCLKRATE 0x38002

#define MBOX_TAG_SETPHYWH 0x48003
#define MBOX_TAG_SETVIRTWH 0x48004
#define MBOX_TAG_SETVIRTOFF 0x48009
#define MBOX_TAG_SETDEPTH 0x48005
#define MBOX_TAG_SETPXLORDR 0x48006
#define MBOX_TAG_GETFB 0x40001
#define MBOX_TAG_GETPITCH 0x40008

#define MBOX_TAG_LAST 0

#define VIDEOCORE_MBOX (MMIO_BASE + 0x0000B880)

#define MBOX_READ ((volatile uint32_t *)(VIDEOCORE_MBOX + 0x0))
#define MBOX_POLL ((volatile uint32_t *)(VIDEOCORE_MBOX + 0x10))
#define MBOX_SENDER ((volatile uint32_t *)(VIDEOCORE_MBOX + 0x14))
#define MBOX_STATUS ((volatile uint32_t *)(VIDEOCORE_MBOX + 0x18))
#define MBOX_CONFIG ((volatile uint32_t *)(VIDEOCORE_MBOX + 0x1C))
#define MBOX_WRITE ((volatile uint32_t *)(VIDEOCORE_MBOX + 0x20))

#define MBOX_RESPONSE 0x80000000
#define MBOX_FULL 0x80000000
#define MBOX_EMPTY 0x40000000

volatile uint32_t mbox[36];

int mbox_call(uint8_t ch) {
    uint32_t r = (((uint32_t)((uint64_t)&mbox) & ~0xF) | (ch & 0xF));

    do {
        asm volatile("nop");
    } while (*MBOX_STATUS & MBOX_FULL);

    *MBOX_WRITE = r;

    while (1) {
        do {
            asm volatile("nop");
        } while (*MBOX_STATUS & MBOX_EMPTY);

        if (r == *MBOX_READ)
            return mbox[1] == MBOX_RESPONSE;
    }

    return 0;
}

#endif
