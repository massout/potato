#include "fb.h"
#include "mem.h"
#include "timer.h"

void main() {
    fb_init();

    draw_circle(100, 100, 50, 10, 1);

    draw_string(300, 300, "asdadasdads", 14);

    move_rect_imm(100, 100, 50, 50, 200, 200, 0);

    move_rect_tp(300, 300, 20, 10, 90, 90, 0);

    while (1)
        ;
}

/*
void main() {
    fb_init();
    char arr[128];

    memset(arr, ' ', 127);

    while (1) {
        for (int i = 0; i < 128; i++) {
            memset(arr, 219, i);
            drawString(0, 8 * (i & 0xf), arr, i & 0xf);
            sleep_sec(1);
        }

        memset(arr, ' ', 127);
    }
}
*/
