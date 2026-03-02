#include "ScreenManager.h"

// Globale Variable
unsigned char current_screen = 0;

void set_current_screen(unsigned char screen_id) {
    current_screen = screen_id;
}

unsigned char get_current_screen(void) {
    return current_screen;
}

