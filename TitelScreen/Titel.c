#include <conio.h>
#include "Titel.h"
#include "../ScreenLoader/ScreenLoader.h"
#include "../ScreenLoader/Screens.h"
#include "../OptionsScreen/Options.h"
#include "../ScreenManager/ScreenManager.h"

void show_titel_screen(void) {
    clrscr();
    load_screen(StartScreen);
    set_current_screen(SCREEN_TITEL);
}

void update_titel_screen(void) {
    update_scroller("      made by florian sass (c) 2026          ", 22, 80);
}

void handle_titel_input(char key) {
    if (key == '1') {
        // Später: start_game();
    }
    else if (key == 33) {
        show_options_menu();
    }
}