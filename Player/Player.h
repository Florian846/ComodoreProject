#ifndef PLAYER_H
#define PLAYER_H

#include "../Card/Card.h"

// Maximale Anzahl Karten pro Spieler
#define MAX_PLAYER_CARDS 11

// Einsatz-Grenzen
#define MIN_BET 10
#define MAX_BET 500
#define BET_STEP 10

// Player Struktur
typedef struct {
    unsigned char id;                    // Spieler ID (0-7)
    unsigned int money;                  // Geld des Spielers
    unsigned int current_bet;            // Aktueller Einsatz dieser Runde
    Card cards[MAX_PLAYER_CARDS];        // Karten des Spielers
    unsigned char card_count;            // Aktuelle Kartenanzahl
    unsigned char is_computer;           // 1 wenn KI-Spieler, 0 wenn Mensch
} Player;

// Player Funktionen
Player* create_player(unsigned char id, unsigned int starting_money, unsigned char is_computer);
void destroy_player(Player* player);
void add_card_to_player(Player* player, Card card);
void remove_card_from_player(Player* player, unsigned char card_index);
void clear_player_hand(Player* player);
unsigned int get_player_money(Player* player);
void set_player_money(Player* player, unsigned int amount);
void add_money_to_player(Player* player, unsigned int amount);
unsigned char remove_money_from_player(Player* player, unsigned int amount);
void set_player_bet(Player* player, unsigned int bet);
unsigned int get_player_bet(Player* player);
void clear_player_bet(Player* player);
unsigned char get_player_card_count(Player* player);
Card* get_player_card(Player* player, unsigned char index);
void print_player(Player* player);

#endif

