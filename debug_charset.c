#include <conio.h>
#include <peekpoke.h>

/* zeigt alle screencodes 0-255 via POKE in screen-ram an.
   druecke eine taste fuer die naechste seite. */

#define SCREEN_RAM 0x8000

void debug_show_charset(void) {
    unsigned int i;
    unsigned char page;
    unsigned char x;
    unsigned char y;

    for (page = 0; page < 16; page++) {
        clrscr();
        cprintf("screencodes %d-%d\r\n\r\n", page * 16, page * 16 + 15);

        for (i = 0; i < 16; i++) {
            unsigned int code;
            code = page * 16 + i;

            /* zwei zeilen pro code: nummer + zeichen */
            x = (i % 8) * 5;
            y = (i / 8) * 4 + 3;

            gotoxy(x, y);
            cprintf("%3d", code);
            gotoxy(x, y + 1);
            cprintf("= ");
            /* screencode direkt in video-ram */
            POKE(SCREEN_RAM + (y + 1) * 40 + x + 2, (unsigned char)code);
        }

        gotoxy(0, 22);
        cprintf("druecke eine taste...");
        cgetc();
    }
}

