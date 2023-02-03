#ifndef FB_H
#define FB_H

#include "char.h"
#include "intdefs.h"
#include "mbox.h"

uint32_t width, height, pitch, isrgb;
uint8_t *fb;

/**
 * It sets up the mailbox to request the framebuffer, and then calls the mailbox
 */
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

/**
 * It takes the x and y coordinates of a pixel, and the attribute of that pixel, and it writes the
 * attribute to the framebuffer at the appropriate offset
 *
 * @param x The x coordinate of the pixel to draw.
 * @param y The y coordinate of the pixel to draw.
 * @param attr The attribute byte.
 */
void draw_pixel(int x, int y, uint8_t attr) {
    int offs = (y * pitch) + (x * 4);
    *((uint32_t *)(fb + offs)) = vgapal[attr & 0x0f];
}

/**
 * It draws a character at a given position on the screen
 *
 * @param ch The character to draw.
 * @param x The x coordinate of the top left corner of the character
 * @param y The y coordinate of the top left corner of the character.
 * @param attr The attribute byte. The lower 4 bits are the foreground color, and the upper 4 bits are
 * the background color.
 */
void draw_char(uint8_t ch, int x, int y, uint8_t attr) {
    uint8_t *glyph = (uint8_t *)&font + (ch < FONT_NUMGLYPHS ? ch : 0) * FONT_BPG;
    uint8_t mask, col;

    for (int i = 0; i < FONT_HEIGHT; i++) {
        for (int j = 0; j < FONT_WIDTH; j++) {
            mask = 1 << j;
            col = (*glyph & mask) ? attr & 0x0f : (attr & 0xf) >> 4;

            draw_pixel(x + j, y + i, col);
        }

        glyph += FONT_BPL;
    }
}

/**
 * It draws a string of characters on the screen
 *
 * @param x The x coordinate of the top left corner of the first character.
 * @param y The y coordinate of the top left corner of the first character.
 * @param s The string to draw
 * @param attr The attribute byte.
 */
void draw_string(int x, int y, char *s, uint8_t attr) {
    while (*s) {
        if (*s == '\r') {
            x = 0;
        } else if (*s == '\n') {
            x = 0;
            y += FONT_HEIGHT;
        } else {
            draw_char(*s, x, y, attr);
            x += FONT_WIDTH;
        }

        s++;
    }
}

/**
 * It draws a rectangle
 *
 * @param x1 The x coordinate of the top left corner of the rectangle
 * @param y1 The y coordinate of the top left corner of the rectangle
 * @param x2 The x coordinate of the bottom right corner of the rectangle
 * @param y2 The y coordinate of the bottom right corner of the rectangle.
 * @param attr The attribute byte.
 * @param fill 0 = no fill, 1 = fill
 */
void draw_rect(int x1, int y1, int x2, int y2, uint8_t attr, uint8_t fill) {
    int y = y1, x;

    while (y <= y2) {
        x = x1;

        while (x <= x2) {
            if ((x == x1 || x == x2) || (y == y1 || y == y2))
                draw_pixel(x, y, attr);
            else if (fill)
                draw_pixel(x, y, (attr & 0xf));

            x++;
        }

        y++;
    }
}

/**
 * If the slope is greater than 1, then we swap the x and y coordinates and draw the line
 *
 * @param x1 The x coordinate of the first point
 * @param y1 The y coordinate of the first point
 * @param x2 The x coordinate of the end point of the line
 * @param y2 The y coordinate of the end point of the line.
 * @param attr The attribute of the pixel.
 */
void draw_line(int x1, int y1, int x2, int y2, uint8_t attr) {
    int dx, dy, p, x, y;

    dx = x2 - x1;
    dy = y2 - y1;
    x = x1;
    y = y1;
    p = 2 * dy - dx;

    while (x < x2) {
        if (p >= 0) {
            draw_pixel(x, y, attr);
            y++;
            p = p + 2 * dy - 2 * dx;
        } else {
            draw_pixel(x, y, attr);
            p = p + 2 * dy;
        }

        x++;
    }
}

/**
 * "Draw a circle by drawing a bunch of lines."
 *
 * The function starts by setting the initial values of the x and y coordinates to the radius of the
 * circle. The err variable is used to keep track of the error between the actual and the desired
 * radius
 *
 * @param x0 The x coordinate of the center of the circle
 * @param y0 The y coordinate of the center of the circle
 * @param radius The radius of the circle
 * @param attr The attribute byte.
 * @param fill 0 = hollow, 1 = filled
 */
void draw_circle(int x0, int y0, int radius, uint8_t attr, uint8_t fill) {
    int x = radius;
    int y = 0;
    int err = 0;

    while (x >= y) {
        if (fill) {
            draw_line(x0 - y, y0 + x, x0 + y, y0 + x, (attr & 0xf));
            draw_line(x0 - x, y0 + y, x0 + x, y0 + y, (attr & 0xf));
            draw_line(x0 - x, y0 - y, x0 + x, y0 - y, (attr & 0xf));
            draw_line(x0 - y, y0 - x, x0 + y, y0 - x, (attr & 0xf));
        }

        draw_pixel(x0 - y, y0 + x, attr);
        draw_pixel(x0 + y, y0 + x, attr);
        draw_pixel(x0 - x, y0 + y, attr);
        draw_pixel(x0 + x, y0 + y, attr);
        draw_pixel(x0 - x, y0 - y, attr);
        draw_pixel(x0 + x, y0 - y, attr);
        draw_pixel(x0 - y, y0 - x, attr);
        draw_pixel(x0 + y, y0 - x, attr);

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

/**
 * It moves a rectangle from one place to another
 *
 * @param oldx The old x coordinate of the rectangle
 * @param oldy The old y coordinate of the rectangle
 * @param width Width of the rectangle
 * @param height height of the rectangle
 * @param newx The new x coordinate of the rectangle
 * @param newy The new y coordinate of the rectangle
 * @param attr The attribute byte.
 * @param transparency 0 = opaque, 1 = transparent
 */
void move_rect(int oldx, int oldy, uint32_t width, uint32_t height, int newx, int newy, uint8_t attr, uint8_t transparency) {
    int new_offs, old_offs;

    for (uint32_t i = 0; i < height; i++) {
        for (uint32_t j = 0; j < width; j++) {
            old_offs = ((oldy + i) * pitch) + ((oldx + j) * 4);
            new_offs = ((newy + i) * pitch) + ((newx + j) * 4);

            if (transparency)
                *((uint32_t *)(fb + new_offs)) |= *((uint32_t *)(fb + old_offs));
            else
                *((uint32_t *)(fb + new_offs)) = *((uint32_t *)(fb + old_offs));
        }
    }

    // Wipe it out old rectangle with attr colour
    draw_rect(oldx, oldy, oldx + width, oldy + width, attr, 1);
}

#endif
