#ifndef SCREEN_MANAGER_H
#define SCREEN_MANAGER_H

// Screen-IDs
#define SCREEN_TITEL 0
#define SCREEN_OPTIONS 1
#define SCREEN_GAME 2

// Globale Variable für aktuellen Screen
extern unsigned char current_screen;

// Funktionen zum Screen-Wechsel
void set_current_screen(unsigned char screen_id);
unsigned char get_current_screen(void);

#endif

