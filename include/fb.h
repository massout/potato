#ifndef FB_H
#define FB_H

#include "char.h"
#include "intdefs.h"
#include "mbox.h"

uint32_t width, height, pitch, isrgb;
uint8_t *fb;

void fb_init() {
    mbox[0] = 35 * 4;
    mbox[1] = MBOX_REQUEST;

    mbox[2] = MBOX_TAG_SETPHYWH;
    mbox[3] = 8;
    mbox[4] = 0;
    mbox[5] = 1920;
    mbox[6] = 1080;

    mbox[7] = MBOX_TAG_SETVIRTWH;
    mbox[8] = 8;
    mbox[9] = 8;
    mbox[10] = 1920;
    mbox[11] = 1080;

    mbox[12] = MBOX_TAG_SETVIRTOFF;
    mbox[13] = 8;
    mbox[14] = 8;
    mbox[15] = 0;
    mbox[16] = 0;

    mbox[17] = MBOX_TAG_SETDEPTH;
    mbox[18] = 4;
    mbox[19] = 4;
    mbox[20] = 32;

    mbox[21] = MBOX_TAG_SETPXLORDR;
    mbox[22] = 4;
    mbox[23] = 4;
    mbox[24] = 1;

    mbox[25] = MBOX_TAG_GETFB;
    mbox[26] = 8;
    mbox[27] = 8;
    mbox[28] = 4096;
    mbox[29] = 0;

    mbox[30] = MBOX_TAG_GETPITCH;
    mbox[31] = 4;
    mbox[32] = 4;
    mbox[33] = 0;

    mbox[34] = MBOX_TAG_LAST;

    if (mbox_call(MBOX_CH_PROP) && mbox[20] == 32 && mbox[28] != 0) {
        mbox[28] &= 0x3FFFFFFF;
        width = mbox[10];
        height = mbox[11];
        pitch = mbox[33];
        isrgb = mbox[24];

        fb = (uint8_t *)((long)mbox[28]);
    }
}

void drawPixel(int x, int y, uint8_t attr) {
    int offs = (y * pitch) + (x * 4);
    *((uint32_t *)(fb + offs)) = vgapal[attr & 0x0f];
}

void drawChar(uint8_t ch, int x, int y, uint8_t attr) {
    uint8_t *glyph = (uint8_t *)&font + (ch < FONT_NUMGLYPHS ? ch : 0) * FONT_BPG;

    for (int i = 0; i < FONT_HEIGHT; i++) {
        for (int j = 0; j < FONT_WIDTH; j++) {
            uint8_t mask = 1 << j;
            uint8_t col = (*glyph & mask) ? attr & 0x0f : (attr & 0xf) >> 4;

            drawPixel(x + j, y + i, col);
        }

        glyph += FONT_BPL;
    }
}

void drawString(int x, int y, char *s, uint8_t attr) {
    while (*s) {
        if (*s == '\r') {
            x = 0;
        } else if (*s == '\n') {
            x = 0;
            y += FONT_HEIGHT;
        } else {
            drawChar(*s, x, y, attr);
            x += FONT_WIDTH;
        }

        s++;
    }
}

void drawRect(int x1, int y1, int x2, int y2, uint8_t attr, int fill) {
    int y = y1;

    while (y <= y2) {
        int x = x1;

        while (x <= x2) {
            if ((x == x1 || x == x2) || (y == y1 || y == y2))
                drawPixel(x, y, attr);
            else if (fill)
                drawPixel(x, y, (attr & 0xf) >> 4);

            x++;
        }
        y++;
    }
}

void drawLine(int x1, int y1, int x2, int y2, uint8_t attr) {
    int dx, dy, p, x, y;

    dx = x2 - x1;
    dy = y2 - y1;
    x = x1;
    y = y1;
    p = 2 * dy - dx;

    while (x < x2) {
        if (p >= 0) {
            drawPixel(x, y, attr);
            y++;
            p = p + 2 * dy - 2 * dx;
        } else {
            drawPixel(x, y, attr);
            p = p + 2 * dy;
        }

        x++;
    }
}

void drawCircle(int x0, int y0, int radius, uint8_t attr, int fill) {
    int x = radius;
    int y = 0;
    int err = 0;

    while (x >= y) {
        if (fill) {
            drawLine(x0 - y, y0 + x, x0 + y, y0 + x, (attr & 0xf) >> 4);
            drawLine(x0 - x, y0 + y, x0 + x, y0 + y, (attr & 0xf) >> 4);
            drawLine(x0 - x, y0 - y, x0 + x, y0 - y, (attr & 0xf) >> 4);
            drawLine(x0 - y, y0 - x, x0 + y, y0 - x, (attr & 0xf) >> 4);
        }

        drawPixel(x0 - y, y0 + x, attr);
        drawPixel(x0 + y, y0 + x, attr);
        drawPixel(x0 - x, y0 + y, attr);
        drawPixel(x0 + x, y0 + y, attr);
        drawPixel(x0 - x, y0 - y, attr);
        drawPixel(x0 + x, y0 - y, attr);
        drawPixel(x0 - y, y0 - x, attr);
        drawPixel(x0 + y, y0 - x, attr);

        if (err <= 0) {
            y += 1;
            err += 2 * y + 1;
        }

        if (err > 0) {
            x -= 1;
            err -= 2 * x + 1;
        }
    }
}

#endif
