#include <conio.h>
#include <peekpoke.h>
#include "Demo.h"
#include "../Card/CardManager.h"

#define SCREEN_RAM 0x8000
#define SCREEN_SIZE 1000
#define SCREEN_WIDTH 40
#define MAX_PIXELS 200

/* 7 helligkeitsstufen (hell -> dunkel) */
static const unsigned char shades[] = {
    32,   /* leerzeichen */
    118,  /* kreuz */
    166,  /* kacheln invertiert */
    230,  /* kacheln */
    214,  /* invertiertes kreuz */
    156,  /* invertierter schraegstrich */
    224   /* invertiertes leerzeichen (voller block) */
};

#define NUM_SHADES 7
#define MAX_LEVEL 12
#define SHADE_FULL 6
#define SHADE_EMPTY 0

extern unsigned char DemoScreen[];

static unsigned int pixel_pos[MAX_PIXELS];
static unsigned char pixel_lvl[MAX_PIXELS];
static unsigned char active_idx[MAX_PIXELS];
static unsigned char active_count;
static unsigned char total_pixels;
static unsigned char flip;

/* schneller 8-bit zufall */
static unsigned char fast_rand(void) {
    random_state = (random_state * 73 + 19);
    return random_state;
}

/* screencode fuer level holen und direkt poken */
static void draw_pixel(unsigned char idx) {
    unsigned char lvl;
    unsigned char shade_i;

    lvl = pixel_lvl[idx];
    shade_i = lvl >> 1;

    if ((lvl & 1) && flip) {
        shade_i++;
    }

    POKE(SCREEN_RAM + pixel_pos[idx], shades[shade_i]);
}

/* alle pixel mit einem bestimmten shade zeichnen */
static void draw_all_shade(unsigned char shade_idx) {
    unsigned char i;
    unsigned char code;

    code = shades[shade_idx];
    for (i = 0; i < total_pixels; i++) {
        POKE(SCREEN_RAM + pixel_pos[i], code);
    }
}

/* kurze pause (busy wait) */
static void delay(unsigned int count) {
    unsigned int d;

    for (d = 0; d < count; d++) {
        /* nichts - nur warten */
    }
}

/* ===== effekt 1: aufbau ===== */

static void effect_build(void) {
    unsigned char pick;
    unsigned char idx;
    unsigned char batch;

    active_count = total_pixels;
    flip = 0;

    for (idx = 0; idx < total_pixels; idx++) {
        pixel_lvl[idx] = 0;
        active_idx[idx] = idx;
    }

    while (active_count > 0) {
        flip = !flip;

        batch = active_count >> 2;
        if (batch < 4) batch = 4;
        if (batch > active_count) batch = active_count;

        while (batch > 0) {
            pick = fast_rand() % active_count;
            idx = active_idx[pick];

            pixel_lvl[idx]++;
            draw_pixel(idx);

            if (pixel_lvl[idx] >= MAX_LEVEL) {
                active_count--;
                active_idx[pick] = active_idx[active_count];
            }

            batch--;
        }

        for (pick = 0; pick < active_count; pick++) {
            idx = active_idx[pick];
            if (pixel_lvl[idx] & 1) {
                draw_pixel(idx);
            }
        }
    }

    draw_all_shade(SHADE_FULL);
}

/* ===== effekt 2: blitz ===== */

static void effect_flash(void) {
    unsigned char blink;

    for (blink = 0; blink < 6; blink++) {
        if (blink & 1) {
            draw_all_shade(SHADE_FULL);
        } else {
            draw_all_shade(SHADE_EMPTY);
        }
        delay(800);
    }

    draw_all_shade(SHADE_FULL);
    delay(1500);
}

/* ===== effekt 3: welle ===== */

static void effect_wave(void) {
    unsigned char wave_x;
    unsigned char pass;
    unsigned char i;
    unsigned char px;
    signed char dist;
    unsigned char shade_idx;

    for (pass = 0; pass < 3; pass++) {
        for (wave_x = 0; wave_x < 50; wave_x++) {
            for (i = 0; i < total_pixels; i++) {
                px = pixel_pos[i] % SCREEN_WIDTH;

                dist = wave_x - px;
                if (dist < 0) dist = -dist;

                if (dist < 3) {
                    shade_idx = SHADE_FULL - 2 + (dist);
                } else {
                    shade_idx = SHADE_FULL;
                }

                if (shade_idx >= NUM_SHADES) shade_idx = SHADE_FULL;

                POKE(SCREEN_RAM + pixel_pos[i], shades[shade_idx]);
            }
        }
    }

    draw_all_shade(SHADE_FULL);
    delay(1000);
}

/* ===== effekt 4: aufloesung ===== */

static void effect_dissolve(void) {
    unsigned char pick;
    unsigned char idx;
    unsigned char batch;

    active_count = total_pixels;

    for (idx = 0; idx < total_pixels; idx++) {
        pixel_lvl[idx] = MAX_LEVEL;
        active_idx[idx] = idx;
    }

    flip = 0;

    while (active_count > 0) {
        flip = !flip;

        batch = active_count >> 2;
        if (batch < 4) batch = 4;
        if (batch > active_count) batch = active_count;

        while (batch > 0) {
            pick = fast_rand() % active_count;
            idx = active_idx[pick];

            if (pixel_lvl[idx] > 0) {
                pixel_lvl[idx]--;
                draw_pixel(idx);
            }

            if (pixel_lvl[idx] == 0) {
                POKE(SCREEN_RAM + pixel_pos[idx], 32);
                active_count--;
                active_idx[pick] = active_idx[active_count];
            }

            batch--;
        }

        for (pick = 0; pick < active_count; pick++) {
            idx = active_idx[pick];
            if (pixel_lvl[idx] & 1) {
                draw_pixel(idx);
            }
        }
    }
}

/* ===== hauptfunktion ===== */

void show_startup_demo(void) {
    unsigned int pos;

    /* alle 1-positionen sammeln */
    total_pixels = 0;
    for (pos = 0; pos < SCREEN_SIZE; pos++) {
        if (DemoScreen[pos] == 1) {
            if (total_pixels < MAX_PIXELS) {
                pixel_pos[total_pixels] = pos;
                total_pixels++;
            }
        }
    }

    clrscr();

    /* 1. text baut sich zufaellig auf */
    effect_build();

    /* 2. blitz-effekt */
    effect_flash();

    /* 3. welle laeuft durch den text */
    effect_wave();

    /* 4. text loest sich zufaellig auf */
    effect_dissolve();

    delay(500);
    clrscr();
}

