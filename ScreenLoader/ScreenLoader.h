#ifndef SCREEN_LOADER_H
#define SCREEN_LOADER_H

#define SCREEN_WIDTH 40
#define VIDEO_START ((unsigned char*)0x8000)

// Lädt ein beliebiges Array in den Videospeicher
void load_screen(const unsigned char* screen_data);

void update_scroller(const char* message, unsigned char row, unsigned char speed);

#endif