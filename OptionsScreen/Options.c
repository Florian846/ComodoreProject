#include <conio.h>
#include "Options.h"
#include "../ScreenLoader/ScreenLoader.h"
#include "../ScreenLoader/Screens.h"
#include "../ScreenManager/ScreenManager.h"

void show_options_menu(void) {
    clrscr();
    load_screen(OptionsScreen);
    set_current_screen(SCREEN_OPTIONS);
}