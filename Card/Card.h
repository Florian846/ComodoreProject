#ifndef CARD_H
#define CARD_H

// Kartenfarben (1-4)
#define SUIT_HEARTS     1
#define SUIT_DIAMONDS   2
#define SUIT_CLUBS      3
#define SUIT_SPADES     4

// Kartenwerte als Zeichen
typedef unsigned char CardValue;

#define VALUE_2         '2'
#define VALUE_3         '3'
#define VALUE_4         '4'
#define VALUE_5         '5'
#define VALUE_6         '6'
#define VALUE_7         '7'
#define VALUE_8         '8'
#define VALUE_9         '9'
#define VALUE_10        'X'  // X für 10
#define VALUE_J         'J'
#define VALUE_Q         'D'  // D für Dame
#define VALUE_K         'K'
#define VALUE_A         'A'

// Kartenstruktur
typedef struct {
    unsigned char suit;      // 1-4 (Herz, Karo, Kreuz, Pik)
    CardValue value;         // '2'-'9', 'X', 'J', 'D', 'K', 'A'
} Card;

// Kartenfunktionen
Card create_card(unsigned char suit, CardValue value);
unsigned char get_card_value_points(Card card);
void print_card(Card card);

#endif

