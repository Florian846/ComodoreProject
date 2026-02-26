#include <conio.h>
#include "Game.h"
#include "../GameConfig.h"
#include "../ScreenManager/ScreenManager.h"
#include "../Card/CardManager.h"
#include "../Player/Player.h"

#define MAX_PLAYERS 8
#define DEFAULT_BET 100
#define DEALER_STAND_VALUE 17
#define BLACKJACK_VALUE 21

typedef struct {
    unsigned char active_player;
    unsigned char round;
    unsigned int pot;
} GameState;

static GameState game_state;
static CardManager* card_manager = 0;
static Player* players[MAX_PLAYERS];
static unsigned char total_players = 0;

/* ===== hilfsfunktionen ===== */

static void wait_for_key(void) {
    cprintf("druecke eine taste...\r\n");
    cgetc();
}

static void print_separator(const char* title) {
    cprintf("=== %s ===\r\n", title);
}

static void print_remaining_cards(void) {
    cprintf("[stapel: %d karten]\r\n", get_remaining_cards(card_manager));
}

static void print_player_standings(void) {
    unsigned char i;

    /* dealer (index 0) ueberspringen */
    for (i = 1; i < total_players; i++) {
        if (players[i]) {
            if (players[i]->is_computer) {
                cprintf("bot %d: $%u\r\n", i, get_player_money(players[i]));
            } else {
                cprintf("spieler %d: $%u\r\n", i, get_player_money(players[i]));
            }
        }
    }
}

/* ===== initialisierung ===== */

void initialize_game_state(void) {
    game_state.active_player = 0;
    game_state.round = 1;
    game_state.pot = 0;
}

void initialize_players(void) {
    unsigned char i;
    unsigned char count;

    /* dealer ist immer players[0] */
    players[0] = create_player(0, 0, 1);
    if (players[0]) {
        cprintf("dealer erstellt\r\n");
    }

    /* menschliche spieler ab index 1 */
    for (i = 0; i < playerCount && (i + 1) < MAX_PLAYERS; i++) {
        players[i + 1] = create_player(i + 1, startingMoney, 0);
        if (players[i + 1]) {
            cprintf("spieler %d erstellt: $%u\r\n", i + 1, startingMoney);
        }
    }

    /* bots nach den menschlichen spielern */
    for (i = 0; i < computerPlayerCount && (playerCount + 1 + i) < MAX_PLAYERS; i++) {
        players[playerCount + 1 + i] = create_player(playerCount + 1 + i, startingMoney, 1);
        if (players[playerCount + 1 + i]) {
            cprintf("bot %d erstellt: $%u\r\n", i + 1, startingMoney);
        }
    }

    /* dealer + spieler + bots */
    count = 1 + playerCount + computerPlayerCount;
    if (count > MAX_PLAYERS) {
        count = MAX_PLAYERS;
    }

    total_players = count;
}

static unsigned char initialize_card_manager(void) {
    Card* cards;
    unsigned char card_count;

    cprintf("erstelle kartenstapel...\r\n");
    card_manager = create_card_manager(cardDecks);

    if (!card_manager) {
        cprintf("fehler: kartenstapel konnte nicht erstellt werden!\r\n");
        return 0;
    }

    cards = get_all_cards(card_manager, &card_count);
    cprintf("kartenstapel erstellt: %d karten\r\n", card_count);
    cprintf("mische karten...\r\n");

    shuffle_cards(cards, card_count);
    cprintf("karten gemischt!\r\n");

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

/* ===== hand-berechnung ===== */

unsigned char calculate_hand_value(Player* player) {
    unsigned char i;
    unsigned char value;
    unsigned char ace_count;
    unsigned char card_val;

    if (!player) return 0;

    value = 0;
    ace_count = 0;

    for (i = 0; i < player->card_count; i++) {
        card_val = get_card_value_points(player->cards[i]);
        if (player->cards[i].value == VALUE_A) {
            ace_count++;
        }
        value += card_val;
    }

    while (value > BLACKJACK_VALUE && ace_count > 0) {
        value -= 10;
        ace_count--;
    }

    return value;
}

static unsigned char is_bust(Player* player) {
    return calculate_hand_value(player) > BLACKJACK_VALUE;
}

static unsigned char is_blackjack(Player* player) {
    if (!player || player->card_count != 2) return 0;
    return calculate_hand_value(player) == BLACKJACK_VALUE;
}

/* ===== karten-anzeige ===== */

static void show_player_hand(Player* player) {
    unsigned char i;

    if (!player) return;

    cprintf("karten: ");
    for (i = 0; i < player->card_count; i++) {
        print_card(player->cards[i]);
        cputc(' ');
    }
    cprintf("(%d punkte)\r\n", calculate_hand_value(player));
}

static void show_dealer_hand_hidden(void) {
    Player* dealer;

    dealer = players[0];
    if (!dealer || dealer->card_count < 2) return;

    cprintf("dealer: ");
    print_card(dealer->cards[0]);
    cprintf(" [??]");
    cprintf("\r\n");
}

static void show_dealer_hand_full(void) {
    Player* dealer;
    unsigned char i;

    dealer = players[0];
    if (!dealer) return;

    cprintf("dealer: ");
    for (i = 0; i < dealer->card_count; i++) {
        print_card(dealer->cards[i]);
        cputc(' ');
    }
    cprintf("(%d punkte)\r\n", calculate_hand_value(dealer));
}

/* ===== karten austeilen ===== */

static void deal_two_cards_to_player(unsigned char index) {
    Card* card;

    card = get_card(card_manager);
    if (card) add_card_to_player(players[index], *card);

    card = get_card(card_manager);
    if (card) add_card_to_player(players[index], *card);
}

static void deal_initial_cards(void) {
    unsigned char i;

    if (!card_manager) return;

    for (i = 0; i < total_players; i++) {
        deal_two_cards_to_player(i);
    }

    cprintf("karten ausgeteilt!\r\n");
}

static Card* draw_card_for_player(Player* player) {
    Card* card;

    card = get_card(card_manager);
    if (card) {
        add_card_to_player(player, *card);
    }
    return card;
}

/* ===== spieler-zug ===== */

static void handle_hit(Player* player) {
    Card* card;

    card = draw_card_for_player(player);
    cprintf("neue karte: ");
    if (card) print_card(*card);
    cputc('\r');
    cputc('\n');
    cprintf("neue summe: %d\r\n", calculate_hand_value(player));
}

static void player_turn(unsigned char player_index) {
    Player* player;
    unsigned char choice;

    player = get_player(player_index);
    if (!player || player->is_computer) return;

    clrscr();
    cprintf("=== spieler %d - zug ===\r\n\r\n", player_index);
    show_dealer_hand_hidden();
    print_remaining_cards();
    cprintf("\r\n");
    show_player_hand(player);

    if (is_blackjack(player)) {
        cprintf("\r\nblackjack! 21 mit 2 karten!\r\n");
        cgetc();
        return;
    }

    while (calculate_hand_value(player) < BLACKJACK_VALUE) {
        cprintf("\r\n1=hit, 2=stand: ");
        choice = cgetc();
        cputc('\r');
        cputc('\n');

        if (choice == 34) {
            handle_hit(player);
            if (is_bust(player)) {
                cprintf("bust! ueber 21!\r\n");
                cgetc();
                return;
            }
        } else if (choice == 33) {
            cprintf("stand!\r\n");
            cgetc();
            return;
        }
    }

    cprintf("21 erreicht!\r\n");
    cgetc();
}

static void computer_turn(unsigned char player_index) {
    Player* player;

    player = get_player(player_index);
    if (!player || !player->is_computer) return;

    while (calculate_hand_value(player) < DEALER_STAND_VALUE) {
        draw_card_for_player(player);
        if (is_bust(player)) return;
    }
}

/* ===== ergebnis-auswertung ===== */

static void evaluate_player_vs_dealer(unsigned char index, unsigned char dealer_value) {
    unsigned char player_value;
    unsigned int bet;
    const char* name;

    player_value = calculate_hand_value(players[index]);
    bet = get_player_bet(players[index]);
    name = players[index]->is_computer ? "bot" : "spieler";

    if (is_bust(players[index])) {
        cprintf("%s %d: bust (-$%u)\r\n", name, index, bet);
    } else if (is_blackjack(players[index]) && !is_blackjack(players[0])) {
        add_money_to_player(players[index], bet + (bet * 3 / 2));
        cprintf("%s %d: blackjack! +$%u\r\n", name, index, bet * 3 / 2);
    } else if (player_value > dealer_value) {
        add_money_to_player(players[index], bet * 2);
        cprintf("%s %d gewinnt! +$%u\r\n", name, index, bet);
    } else if (player_value == dealer_value) {
        add_money_to_player(players[index], bet);
        cprintf("%s %d: push (einsatz zurueck)\r\n", name, index);
    } else {
        cprintf("%s %d verliert (-$%u)\r\n", name, index, bet);
    }
}

static void evaluate_dealer_bust(void) {
    unsigned char i;
    unsigned int bet;
    const char* name;

    cprintf("dealer hat bust! alle verbleibenden spieler gewinnen!\r\n");

    for (i = 1; i < total_players; i++) {
        if (!is_bust(players[i])) {
            bet = get_player_bet(players[i]);
            name = players[i]->is_computer ? "bot" : "spieler";
            add_money_to_player(players[i], bet * 2);
            cprintf("%s %d gewinnt: +$%u\r\n", name, i, bet);
        }
    }
}

static void determine_winners(void) {
    unsigned char i;
    unsigned char dealer_value;
    const char* name;

    dealer_value = calculate_hand_value(players[0]);

    clrscr();
    print_separator("ergebnis");
    cprintf("\r\n");
    show_dealer_hand_full();
    cprintf("\r\n");

    for (i = 1; i < total_players; i++) {
        name = players[i]->is_computer ? "bot" : "spieler";
        cprintf("%s %d: ", name, i);
        show_player_hand(players[i]);
    }
    cprintf("\r\n");

    if (is_bust(players[0])) {
        evaluate_dealer_bust();
    } else {
        for (i = 1; i < total_players; i++) {
            evaluate_player_vs_dealer(i, dealer_value);
        }
    }

    cgetc();
}

/* ===== aufraeum-funktionen ===== */

static void clear_all_hands(void) {
    unsigned char i;

    for (i = 0; i < total_players; i++) {
        clear_player_hand(players[i]);
        clear_player_bet(players[i]);
    }
}

static unsigned char check_if_all_out(void) {
    unsigned char i;
    unsigned char bankrupt;

    bankrupt = 0;

    /* dealer (index 0) ueberspringen */
    for (i = 1; i < total_players; i++) {
        if (get_player_money(players[i]) <= 0) {
            bankrupt++;
        }
    }

    return bankrupt == (total_players - 1);
}

/* ===== spielphasen ===== */

static void place_human_bet(unsigned char index) {
    Player* player;
    unsigned int bet;
    unsigned int max_allowed;
    unsigned char key;

    player = players[index];
    bet = MIN_BET;
    max_allowed = get_player_money(player);

    if (max_allowed > MAX_BET) {
        max_allowed = MAX_BET;
    }
    if (max_allowed < MIN_BET) {
        max_allowed = get_player_money(player);
    }

    clrscr();
    cprintf("=== spieler %d - einsatz ===\r\n", index);
    cprintf("geld: $%u\r\n\r\n", get_player_money(player));
    cprintf("1/2 = einsatz +/- %d\r\n", BET_STEP);
    cprintf("3/4 = einsatz +/- %d\r\n", BET_STEP * 5);
    cprintf("5   = bestaetigen\r\n\r\n");

    while (1) {
        gotoxy(0, 8);
        cprintf("einsatz: $%-6u", bet);

        key = cgetc();

        if (key == 34 && bet + BET_STEP <= max_allowed) {
            bet += BET_STEP;
        } else if (key == 33 && bet > MIN_BET + BET_STEP - 1) {
            bet -= BET_STEP;
        } else if (key == 91 && bet + BET_STEP * 5 <= max_allowed) {
            bet += BET_STEP * 5;
        } else if (key == 36 && bet > MIN_BET + BET_STEP * 5 - 1) {
            bet -= BET_STEP * 5;
        } else if (key == 35) {
            break;
        }

        if (bet < MIN_BET) {
            bet = MIN_BET;
        }
        if (bet > max_allowed) {
            bet = max_allowed;
        }
    }

    set_player_bet(player, bet);
    remove_money_from_player(player, bet);
    game_state.pot += bet;

    cprintf("\r\nspieler %d setzt $%u\r\n", index, bet);
}

static void place_computer_bet(unsigned char index) {
    Player* player;
    unsigned int bet;

    player = players[index];
    bet = MIN_BET;

    if (get_player_money(player) >= MIN_BET * 2) {
        bet = MIN_BET * 2;
    }
    if (get_player_money(player) < bet) {
        bet = get_player_money(player);
    }

    set_player_bet(player, bet);
    remove_money_from_player(player, bet);
    game_state.pot += bet;

    cprintf("bot %d setzt $%u\r\n", index + 1, bet);
}

static void game_phase_betting(void) {
    unsigned char i;

    game_state.pot = 0;

    /* dealer (index 0) wettet nicht */
    for (i = 1; i < total_players; i++) {
        if (!players[i]) continue;
        if (get_player_money(players[i]) == 0) continue;

        if (players[i]->is_computer) {
            place_computer_bet(i);
        } else {
            place_human_bet(i);
        }
    }

    clrscr();
    cprintf("alle einsaetze gesetzt!\r\n");
    cprintf("pot: $%u\r\n", game_state.pot);
    wait_for_key();
}

static void game_phase_deal(void) {
    clrscr();
    cprintf("teile karten aus...\r\n");
    deal_initial_cards();
    print_remaining_cards();
    wait_for_key();
}

static void game_phase_player_turns(void) {
    unsigned char i;

    /* dealer (index 0) ueberspringen - spielt in dealer-runde */
    for (i = 1; i < total_players; i++) {
        if (players[i]->is_computer) {
            clrscr();
            cprintf("=== bot %d - zug ===\r\n\r\n", i);
            show_dealer_hand_hidden();
            print_remaining_cards();
            cprintf("\r\n");
            show_player_hand(players[i]);
            computer_turn(i);
            cprintf("\r\n");
            show_player_hand(players[i]);
            if (is_bust(players[i])) {
                cprintf("bot %d: bust!\r\n", i);
            } else {
                cprintf("bot %d steht bei %d\r\n", i, calculate_hand_value(players[i]));
            }
            cgetc();
        } else {
            player_turn(i);
        }
    }
}

static void game_phase_dealer_turn(void) {
    clrscr();
    print_separator("dealer-runde");
    print_remaining_cards();
    cprintf("\r\ndealer deckt auf: ");
    show_dealer_hand_full();

    while (calculate_hand_value(players[0]) < DEALER_STAND_VALUE) {
        draw_card_for_player(players[0]);
        cprintf("dealer zieht: ");
        print_card(players[0]->cards[players[0]->card_count - 1]);
        cputc('\r');
        cputc('\n');
    }

    cprintf("\r\n");
    show_dealer_hand_full();

    if (is_bust(players[0])) {
        cprintf("dealer bust!\r\n");
    }

    wait_for_key();
}

static void game_phase_results(unsigned char round) {
    determine_winners();

    clrscr();
    cprintf("spielerstaende nach runde %d:\r\n\r\n", round);
    print_player_standings();
    cprintf("\r\n");
    wait_for_key();
}

static void game_phase_cleanup_round(void) {
    clear_all_hands();
}

static unsigned char game_phase_check_status(void) {
    unsigned char all_out;

    clrscr();
    all_out = check_if_all_out();

    if (all_out) {
        cprintf("alle spieler sind pleite! spiel vorbei!\r\n");
    } else {
        cprintf("das spiel geht weiter...\r\n");
    }

    wait_for_key();
    return all_out;
}

static void game_phase_show_final_results(void) {
    clrscr();
    print_separator("spiel vorbei");
    cprintf("\r\n");
    print_player_standings();
    cprintf("\r\ndruecke eine taste um zum hauptmenue zurueckzukehren...\r\n");
    cgetc();
}

static void show_game_intro(void) {
    cprintf("\r\nspiel gestartet!\r\n");
    cprintf("%d spieler, %d bots, %d deck(s)\r\n",
            playerCount, computerPlayerCount, cardDecks);
    wait_for_key();
}

/* ===== hauptsteuerung ===== */

void start_game(void) {
    unsigned char round;
    unsigned char all_out;

    round = 1;
    all_out = 0;

    if (!initialize_game()) {
        cprintf("fehler beim initialisieren!\r\n");
        cgetc();
        return;
    }

    show_game_intro();
    set_current_screen(SCREEN_GAME);

    while (!all_out) {
        game_phase_betting();
        game_phase_deal();
        game_phase_player_turns();
        game_phase_dealer_turn();
        game_phase_results(round);
        game_phase_cleanup_round();
        all_out = game_phase_check_status();

        if (!all_out) {
            round++;
        }
    }

    game_phase_show_final_results();
    cleanup_game();
    set_current_screen(SCREEN_TITEL);
}

/* ===== zugriffsfunktionen ===== */

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

