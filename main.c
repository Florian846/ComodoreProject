#include <conio.h>
#include "TitelScreen/Titel.h"

int main(void) {
    cputc(142);
    show_titel_screen();

    while(1) {
        if (kbhit()) {
            handle_titel_input(cgetc());
        }

        update_titel_screen();
    }

    return 0;
}