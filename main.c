// main.c
#include <conio.h>

extern unsigned char StartScreen[]; // Das 'extern' ist hier wichtig!

void draw_frame(void) {
    int i;
    unsigned char ch;
    for (i = 0; i < 1000; ++i) {
        ch = StartScreen[i];
        if (ch >= 128) {
            revers(1);         // Invertieren an
            cputc(ch - 128);   // Das Basis-Zeichen zeichnen
            revers(0);         // Invertieren aus
        } else {
            cputc(ch);
        }
    }
}

int main(void) {
    // Schaltet den PET in den Grafik-Modus (Großbuchstaben + Grafik)
    cputc(142);

    clrscr();
    cursor(1);

    draw_frame();

    while(!kbhit());
    return 0;
}