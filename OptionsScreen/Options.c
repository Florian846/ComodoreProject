#include <conio.h>
#include "Options.h"
#include "../ScreenLoader/ScreenLoader.h"
#include "../ScreenLoader/Screens.h"

void show_options_menu(void) {
    clrscr();
    load_screen(OptionsScreen);
}