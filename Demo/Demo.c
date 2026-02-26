#include <conio.h>
#include <peekpoke.h>
#include "Demo.h"
#include "../Card/CardManager.h"

#define SCREEN_RAM 0x8000
#define SCREEN_SIZE 1000
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

/* 13 stufen: 0=leer, 1=interp, 2=kreuz, ... 12=block */
#define MAX_LEVEL 12

extern unsigned char DemoScreen[];

/* positionen und level aller aktiven pixel */
static unsigned int pixel_pos[MAX_PIXELS];
static unsigned char pixel_lvl[MAX_PIXELS];

/* indizes der noch nicht fertigen pixel */
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
    unsigned char code;

    lvl = pixel_lvl[idx];
    shade_i = lvl >> 1;

    /* ungerade level = interpolation */
    if ((lvl & 1) && flip) {
        shade_i++;
    }

    code = shades[shade_i];
    POKE(SCREEN_RAM + pixel_pos[idx], code);
}

void show_startup_demo(void) {
    unsigned int pos;
    unsigned char pick;
    unsigned char idx;
    unsigned char batch;

    /* alle 1-positionen sammeln */
    total_pixels = 0;
    for (pos = 0; pos < SCREEN_SIZE; pos++) {
        if (DemoScreen[pos] == 1) {
            if (total_pixels < MAX_PIXELS) {
                pixel_pos[total_pixels] = pos;
                pixel_lvl[total_pixels] = 0;
                active_idx[total_pixels] = total_pixels;
                total_pixels++;
            }
        }
    }

    clrscr();
    active_count = total_pixels;
    flip = 0;

    while (active_count > 0) {
        flip = !flip;

        /* mehrere pixel pro frame erhoehen */
        batch = active_count >> 2;
        if (batch < 4) batch = 4;
        if (batch > active_count) batch = active_count;

        while (batch > 0) {
            /* zufaelligen aktiven pixel waehlen */
            pick = fast_rand() % active_count;
            idx = active_idx[pick];

            pixel_lvl[idx]++;
            draw_pixel(idx);

            if (pixel_lvl[idx] >= MAX_LEVEL) {
                /* fertig: aus active-liste entfernen (swap mit letztem) */
                active_count--;
                active_idx[pick] = active_idx[active_count];
            }

            batch--;
        }

        /* interpolations-pixel aktualisieren (nur ungerade level) */
        for (pick = 0; pick < active_count; pick++) {
            idx = active_idx[pick];
            if (pixel_lvl[idx] & 1) {
                draw_pixel(idx);
            }
        }
    }

    /* finale: sicherstellen alle auf block */
    for (idx = 0; idx < total_pixels; idx++) {
        POKE(SCREEN_RAM + pixel_pos[idx], 224);
    }

    cgetc();

    clrscr();
}

