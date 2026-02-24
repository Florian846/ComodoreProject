#include <conio.h>
#include "ObjectLoader.h"

void draw_object(const char* charArray, unsigned char objectWidth, unsigned char objectHeight) {
    unsigned int index = 0;
    unsigned char x, y;

    // Speichere die aktuelle Cursor-Position
    unsigned char startX = wherex();
    unsigned char startY = wherey();

    // Zeichne das Objekt Zeile für Zeile
    for (y = 0; y < objectHeight; y++) {
        // Setze den Cursor an den Anfang der aktuellen Zeile
        gotoxy(startX, startY + y);

        // Zeichne die Zeichen für diese Zeile
        for (x = 0; x < objectWidth; x++) {
            cputc(charArray[index]);
            index++;
        }
    }

    // Setze den Cursor zurück zur Startposition (optional)
    gotoxy(startX, startY);
}

