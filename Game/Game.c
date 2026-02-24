#include <conio.h>
#include "Game.h"
#include "../GameConfig.h"
#include "../ScreenManager/ScreenManager.h"
#include "../Card/CardManager.h"
#include "../Player/Player.h"

#define MAX_PLAYERS 8

typedef struct {
    unsigned char active_player;
    unsigned char round;
    unsigned int pot;
} GameState;

// Globale Variablen
static GameState game_state;
static CardManager* card_manager = 0;
static Player* players[MAX_PLAYERS];
static unsigned char total_players = 0;

void initialize_game_state(void) {
    game_state.active_player = 0;
    game_state.round = 1;
    game_state.pot = 0;
}

void initialize_players(void) {
    unsigned char i;
    unsigned char total_players_count = playerCount + computerPlayerCount;

    if (total_players_count > MAX_PLAYERS) {
        total_players_count = MAX_PLAYERS;
    }

    for (i = 0; i < playerCount && i < MAX_PLAYERS; i++) {
        players[i] = create_player(i, startingMoney, 0);
        if (players[i]) {
            cprintf("Spieler %d erstellt: $%u\r\n", i, startingMoney);
        }
    }

    for (i = 0; i < computerPlayerCount && (playerCount + i) < MAX_PLAYERS; i++) {
        players[playerCount + i] = create_player(playerCount + i, startingMoney, 1);
        if (players[playerCount + i]) {
            cprintf("Bot %d erstellt: $%u\r\n", i, startingMoney);
        }
    }

    total_players = total_players_count;
}

unsigned char initialize_card_manager(void) {
    Card* cards;
    unsigned char card_count;

    cprintf("Erstelle Kartenstapel...\r\n");
    card_manager = create_card_manager(cardDecks);

    if (!card_manager) {
        cprintf("Fehler: CardManager konnte nicht erstellt werden!\r\n");
        return 0;
    }

    cards = get_all_cards(card_manager, &card_count);
    cprintf("Kartenstapel erstellt: %d Karten\r\n", card_count);
    cprintf("Mische Karten...\r\n");

    shuffle_cards(cards, card_count);
    cprintf("Karten gemischt!\r\n");

    return 1;
}

unsigned char initialize_game(void) {
    clrscr();

    if (!initialize_card_manager()) {
        cgetc();
        return 0;
    }

    initialize_game_state();
    initialize_players();

    return 1;
}

void start_game(void) {
    clrscr();

    if (!initialize_game()) {
        cprintf("Fehler beim Initialisieren des Spiels!\r\n");
        cgetc();
        return;
    }

    cprintf("\r\nSpiel gestartet mit %d Spielern (%d menschlich, %d KI) und %d Deck(s)\r\n",
            total_players, playerCount, computerPlayerCount, cardDecks);
    cprintf("Druecke eine Taste zum Fortfahren...\r\n");
    cgetc();

    clrscr();
    set_current_screen(SCREEN_GAME);
    cprintf("Willkommen im Spiel!\r\n");
}

Player* get_player(unsigned char index) {
    if (index >= total_players) return 0;
    return players[index];
}

unsigned char get_total_players(void) {
    return total_players;
}

void cleanup_game(void) {
    unsigned char i;

    for (i = 0; i < total_players; i++) {
        if (players[i]) {
            destroy_player(players[i]);
            players[i] = 0;
        }
    }

    if (card_manager) {
        destroy_card_manager(card_manager);
        card_manager = 0;
    }

    total_players = 0;
}

