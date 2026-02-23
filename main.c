#include <conio.h>
#include "TitelScreen/Titel.h"
#include "ScreenManager/ScreenManager.h"

// Globale Spielkonfiguration
unsigned char playerCount = 1;
unsigned char computerPlayerCount = 0;
unsigned int startingMoney = 500;
unsigned char cardDecks = 1;

int main(void) {
    cputc(142);
    set_current_screen(SCREEN_TITEL);
    show_titel_screen();

    while(1) {
        if (kbhit()) {
            handle_titel_input(cgetc());
        }

        // Scroller nur auf Titel-Screen aktualisieren
        if (get_current_screen() == SCREEN_TITEL) {
            update_titel_screen();
        }
    }

    return 0;
}