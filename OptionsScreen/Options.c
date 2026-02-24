#include <conio.h>
#include "Options.h"
#include "../ScreenLoader/ScreenLoader.h"
#include "../ScreenLoader/Screens.h"
#include "../ScreenManager/ScreenManager.h"
#include "../GameConfig.h"
#include "../TitelScreen/Titel.h"

void show_options_menu(void) {
    clrscr();
    load_screen(OptionsScreen);
    set_current_screen(SCREEN_OPTIONS);
    update_options_screen();
}

void update_options_screen(void) {
    // Player Count - Position 33, Zeile 7
    gotoxy(33, 7);
    cprintf("%d ", playerCount);

    // Computer Player - Position 33, Zeile 10
    gotoxy(33, 10);
    cprintf("%d ", computerPlayerCount);

    // Starting Money - Position 31, Zeile 13 (mit fester Breite, rechtsbündig)
    gotoxy(29, 13);
    cprintf("%5u", startingMoney);

    // Card Decks - Position 33, Zeile 16
    gotoxy(33, 16);
    cprintf("%d ", cardDecks);
}

void handle_options_input(char key) {
    if (key == 34) {
        playerCount++;
        if (playerCount > 4) {
            playerCount = 1;
        }
        update_options_screen();
    }
    else if (key == 33) {
        computerPlayerCount++;
        if (computerPlayerCount > 4) {
            computerPlayerCount = 0;
        }
        update_options_screen();
    }
    else if (key == 91) {
        startingMoney += 50;
        if (startingMoney >= 5050) {
            startingMoney = 50;
        }
        update_options_screen();
    }
    else if (key == 36) {
        cardDecks++;
        if (cardDecks > 4) {
            cardDecks = 1;
        }
        update_options_screen();
    }
    else if (key == 35) {
        show_titel_screen();
    }
}