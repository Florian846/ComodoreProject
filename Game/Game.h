#ifndef GAME_H
#define GAME_H

#include "../Player/Player.h"

// Spielinitialisierung und Start
unsigned char initialize_game(void);
void start_game(void);
void initialize_game_state(void);
void initialize_players(void);
void cleanup_game(void);

// Spielerzugriff
Player* get_player(unsigned char index);
unsigned char get_total_players(void);

#endif

