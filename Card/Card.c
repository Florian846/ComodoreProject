#include <conio.h>
#include "Card.h"

// Karte erstellen
Card create_card(unsigned char suit, CardValue value) {
    Card card;
    card.suit = suit;
    card.value = value;
    return card;
}

// Kartenwert in Punkte umrechnen (für Blackjack)
unsigned char get_card_value_points(Card card) {
    switch (card.value) {
        case VALUE_2:
            return 2;
        case VALUE_3:
            return 3;
        case VALUE_4:
            return 4;
        case VALUE_5:
            return 5;
        case VALUE_6:
            return 6;
        case VALUE_7:
            return 7;
        case VALUE_8:
            return 8;
        case VALUE_9:
            return 9;
        case VALUE_10:
            return 10;
        case VALUE_J:
        case VALUE_Q:
        case VALUE_K:
            return 10;
        case VALUE_A:
            return 11;  // Ass zählt 11 (kann später als 1 behandelt werden)
        default:
            return 0;
    }
}

// Karte ausgeben
void print_card(Card card) {
    char suit_char;

    // Farbesymbole
    switch (card.suit) {
        case SUIT_HEARTS:
            suit_char = 3;      // Herz-Symbol
            break;
        case SUIT_DIAMONDS:
            suit_char = 4;      // Karo-Symbol
            break;
        case SUIT_CLUBS:
            suit_char = 5;      // Kreuz-Symbol
            break;
        case SUIT_SPADES:
            suit_char = 6;      // Pik-Symbol
            break;
        default:
            suit_char = '?';
    }

    cputc(card.value);
    cputc(suit_char);
}

