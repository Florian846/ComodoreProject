#ifndef CARD_MANAGER_H
#define CARD_MANAGER_H

#include "Card.h"

// Maximale Anzahl von Karten (52 pro Deck * max 4 Decks)
#define MAX_CARDS 208
#define CARDS_PER_DECK 52

// CardManager Struktur
typedef struct {
    Card cards[MAX_CARDS];      // Kartenstapel
    unsigned char card_count;   // Aktuelle Anzahl Karten
    unsigned char current_index; // Position zum Abheben
} CardManager;

// Zufallsgenerator
extern unsigned char random_state;

// CardManager Funktionen
CardManager* create_card_manager(unsigned char num_stacks);
void destroy_card_manager(CardManager* manager);
Card* get_all_cards(CardManager* manager, unsigned char* count);
void shuffle_cards(Card* cards, unsigned char count);
Card* get_card(CardManager* manager);
unsigned char get_remaining_cards(CardManager* manager);
void print_card_manager(CardManager* manager);

#endif

