#include <conio.h>
#include <stdlib.h>
#include "CardManager.h"

// Statischer Manager (für globale Verwaltung)
static CardManager* global_manager = 0;

// Einfacher Zufallsgenerator ohne time()
static unsigned char random_state = 42;

unsigned char simple_rand(void) {
    random_state = (random_state * 73 + 19) & 0xFF;
    return random_state;
}

// Kartenstapel erstellen mit anzahl Stacks (Decks)
CardManager* create_card_manager(unsigned char num_stacks) {
    unsigned char i, j, k;
    unsigned char suits[] = {SUIT_HEARTS, SUIT_DIAMONDS, SUIT_CLUBS, SUIT_SPADES};
    CardValue values[] = {VALUE_2, VALUE_3, VALUE_4, VALUE_5, VALUE_6, VALUE_7,
                          VALUE_8, VALUE_9, VALUE_10, VALUE_J, VALUE_Q, VALUE_K, VALUE_A};

    CardManager* manager = (CardManager*) malloc(sizeof(CardManager));
    if (!manager) return 0;

    manager->card_count = 0;
    manager->current_index = 0;

    // Limitiere auf max 4 Stacks
    if (num_stacks > 4) num_stacks = 4;
    if (num_stacks < 1) num_stacks = 1;

    // Erstelle alle Karten für jeden Stack
    for (i = 0; i < num_stacks; i++) {
        // 4 Farben
        for (j = 0; j < 4; j++) {
            // 13 Kartenwerte
            for (k = 0; k < 13; k++) {
                if (manager->card_count < MAX_CARDS) {
                    manager->cards[manager->card_count].suit = suits[j];
                    manager->cards[manager->card_count].value = values[k];
                    manager->card_count++;
                }
            }
        }
    }

    // Mische die Karten nach Erstellung
    shuffle_cards(manager->cards, manager->card_count);

    global_manager = manager;
    return manager;
}

// CardManager freigeben
void destroy_card_manager(CardManager* manager) {
    if (manager) {
        free(manager);
        global_manager = 0;
    }
}

// Alle erstellten Karten als Array zurückgeben
Card* get_all_cards(CardManager* manager, unsigned char* count) {
    if (!manager || !count) return 0;
    *count = manager->card_count;
    return manager->cards;
}

// Zufälliges Mischen des Kartenstapels (Fisher-Yates Shuffle)
void shuffle_cards(Card* cards, unsigned char count) {
    unsigned char i, j;
    Card temp;

    if (!cards || count == 0) return;

    // Fisher-Yates Shuffle mit simple_rand()
    for (i = count - 1; i > 0; i--) {
        j = simple_rand() % (i + 1);

        // Tausche cards[i] mit cards[j]
        temp = cards[i];
        cards[i] = cards[j];
        cards[j] = temp;
    }
}

// Oberste Karte vom Deck abheben
Card* get_card(CardManager* manager) {
    if (!manager) return 0;

    // Wenn keine Karten mehr, gib NULL zurück
    if (manager->current_index >= manager->card_count) {
        return 0;
    }

    // Gib Zeiger auf aktuelle Karte und erhöhe Index
    return &manager->cards[manager->current_index++];
}

// Kartenstapel anzeigen (Debug)
void print_card_manager(CardManager* manager) {
    unsigned char i;

    if (!manager) return;

    cprintf("CardManager: %d Karten\r\n", manager->card_count);
    cprintf("Current Index: %d\r\n", manager->current_index);
    cprintf("Remaining Cards: %d\r\n", manager->card_count - manager->current_index);

    // Zeige erste 10 Karten
    for (i = 0; i < 10 && i < manager->card_count; i++) {
        cprintf("Karte %d: ", i);
        print_card(manager->cards[i]);
        cprintf("\r\n");
    }
}

