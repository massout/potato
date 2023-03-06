#include "fb.h"
#include "format.h"
#include "mem.h"
#include "timer.h"

/**
 * It draws the greeting message to the screen
 */
void boot() {
/* A preprocessor directive. It checks if the file `fb.h` has been included. If not, it includes it. */
#ifndef FB_H
#include "fb.h"
#endif

    /* It initializes the framebuffer. */
    fb_init();

    draw_string(50, 100, "                                                                                        ,----, ", 12);
    draw_string(50, 108, "           ____                                          ,----..                      ,/   .`| ", 12);
    draw_string(50, 116, "         ,'  , `.   ,---,       .--.--.    .--.--.      /   /   |                   ,`   .'  : ", 12);
    draw_string(50, 124, "      ,-+-,.' _ |  '  .' |     /  /    '. /  /    '.   /   .     :          ,--,  ;    ;     / ", 12);
    draw_string(50, 132, "   ,-+-. ;   , || /  ;    '.  |  :  /`. /|  :  /`. /  .   /   ;.  |       ,'_ /|.'___,/    ,'  ", 12);
    draw_string(50, 140, "  ,--.'|'   |  ;|:  :       | ;  |  |--` ;  |  |--`  .   ;   /  ` ;  .--. |  | :|    :     |   ", 12);
    draw_string(50, 148, " |   |  ,', |  '::  |   /|   ||  :  ;_   |  :  ;_    ;   |  ; | ; |,'_ /| :  . |;    |.';  ;   ", 12);
    draw_string(50, 156, " |   | /  | |  |||  :  ' ;.   :|  |    `. |  |    `. |   :  | ; | '|  ' | |  . .`----'  |  |   ", 12);
    draw_string(50, 164, " '   | :  | :  |,|  |  ;/| |   |`----.   | `----.   |.   |  ' ' ' :|  | ' |  | |    '   :  ;   ", 12);
    draw_string(50, 172, " ;   . |  ; |--' '  :  | |  | ,'__ |  |  | __ |  |  |'   ;  |; /  |:  | | :  ' ;    |   |  '   ", 12);
    draw_string(50, 180, " |   : |  | ,    |  |  '  '--' /  /`--'  //  /`--'  / |   |  ',  / |  ; ' |  | '    '   :  |   ", 12);
    draw_string(50, 188, " |   : '  |/     |  :  :      '--'.     /'--'.     /   ;   :    /  :  | : ;  ; |    ;   |.'    ", 12);
    draw_string(50, 196, " ;   | |`-'      |  | ,'        `--'---'   `--'---'     |   | .'   '  :  `--'  |    '---'      ", 12);
    draw_string(50, 204, " |   ;/          `--''                                   `---`     :  ,      .-./              ", 12);
    draw_string(50, 212, " '---'                                                              `--`----'                  ", 12);

    draw_string(50, 236, "                                            BOOTING :)                                         ", 12);

/* It checks if the file `timer.h` has been included. If not, it includes it. */
#ifndef TIMER_H
#include "timer.h"
#endif

    sleep_sec(2);

    draw_rect(50, 100, 1000, 500, 0, 1);  // Wipe greeting message out
}

void main() {
    boot();
    char *b = "";

    draw_string(10, 10, itoa(atoi("-256"), b), 12);

    while (1)
        ;
}
