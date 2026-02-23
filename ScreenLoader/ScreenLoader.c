#include <string.h>
#include "ScreenLoader.h"

void load_screen(const unsigned char* screen_data) {
    if (screen_data != NULL) {
        memcpy(VIDEO_START, screen_data, 1000);
    }
}

void update_scroller(const char* message, unsigned char row, unsigned char speed) {
    static int offset = 0;
    static unsigned char speed_ctr = 0;

    int msg_len = strlen(message);
    unsigned char* ptr = VIDEO_START + (row * SCREEN_WIDTH);
    const char* m_ptr;
    int i;

    if (++speed_ctr < speed) return;
    speed_ctr = 0;

    m_ptr = &message[offset];

    for (i = 0; i < SCREEN_WIDTH; ++i) {
        unsigned char c = (unsigned char)*m_ptr++;

        // Wrap around wenn Ende des Strings erreicht
        if (c == '\0') {
            m_ptr = message;
            c = (unsigned char)*m_ptr++;
        }

        // Zeichen-Konvertierung (Screencodes)
        if (c >= 97 && c <= 122) c -= 96;      // Kleinbuchstaben
        else if (c >= 65 && c <= 90) c -= 64; // Großbuchstaben

        *ptr++ = c;
    }

    if (++offset >= msg_len) offset = 0;
}