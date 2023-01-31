#include "fb.h"
#include "mem.h"
#include "timer.h"

void main() {
    fb_init();
    char arr[128];

    memset(arr, ' ', 127);

    drawCircle(500, 500, 20, 10, 1);

    while (1) {
        for (int i = 0; i < 128; i++) {
            memset(arr, 219, i);
            drawString(0, 8 * (i & 0xf), arr, i & 0xf);
            sleep_sec(1);
        }

        memset(arr, ' ', 127);
    }
}