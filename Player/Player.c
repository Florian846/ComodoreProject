#include <conio.h>
#include <stdlib.h>
#include "Player.h"

Player* create_player(unsigned char id, unsigned int starting_money, unsigned char is_computer) {
    Player* player = (Player*) malloc(sizeof(Player));

    if (!player) return 0;

    player->id = id;
    player->money = starting_money;
    player->card_count = 0;
    player->is_computer = is_computer;

    return player;
}

// Spieler freigeben
void destroy_player(Player* player) {
    if (player) {
        free(player);
    }
}

// Karte zu Spieler hinzufügen
void add_card_to_player(Player* player, Card card) {
    if (!player || player->card_count >= MAX_PLAYER_CARDS) {
        return;
    }

    player->cards[player->card_count] = card;
    player->card_count++;
}

// Karte von Spieler entfernen
void remove_card_from_player(Player* player, unsigned char card_index) {
    unsigned char i;

    if (!player || card_index >= player->card_count) {
        return;
    }

    // Verschiebe alle Karten nach dem Index um eins nach vorne
    for (i = card_index; i < player->card_count - 1; i++) {
        player->cards[i] = player->cards[i + 1];
    }

    player->card_count--;
}

// Alle Karten vom Spieler entfernen
void clear_player_hand(Player* player) {
    if (!player) return;
    player->card_count = 0;
}

// Geld des Spielers abrufen
unsigned int get_player_money(Player* player) {
    if (!player) return 0;
    return player->money;
}

// Geld des Spielers setzen
void set_player_money(Player* player, unsigned int amount) {
    if (!player) return;
    player->money = amount;
}

// Geld zu Spieler hinzufügen
void add_money_to_player(Player* player, unsigned int amount) {
    if (!player) return;
    player->money += amount;
}

// Geld von Spieler entfernen (gibt 1 zurück wenn erfolgreich, 0 wenn nicht genug Geld)
unsigned char remove_money_from_player(Player* player, unsigned int amount) {
    if (!player) return 0;

    if (player->money < amount) {
        return 0;  // Nicht genug Geld
    }

    player->money -= amount;
    return 1;  // Erfolgreich
}

// Kartenanzahl des Spielers abrufen
unsigned char get_player_card_count(Player* player) {
    if (!player) return 0;
    return player->card_count;
}

// Spezifische Karte des Spielers abrufen
Card* get_player_card(Player* player, unsigned char index) {
    if (!player || index >= player->card_count) return 0;
    return &player->cards[index];
}

// Spieler ausgeben (Debug)
void print_player(Player* player) {
    unsigned char i;

    if (!player) return;

    cprintf("Spieler %d:\r\n", player->id);
    cprintf("Geld: $%u\r\n", player->money);
    cprintf("Karten: %d\r\n", player->card_count);

    if (player->is_computer) {
        cprintf("Typ: KI\r\n");
    } else {
        cprintf("Typ: Mensch\r\n");
    }

    // Zeige Karten
    for (i = 0; i < player->card_count; i++) {
        cprintf("  ");
        print_card(player->cards[i]);
        cprintf(" ");
    }
    cprintf("\r\n");
}

