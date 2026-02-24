#ifndef OBJECTLOADER_H
#define OBJECTLOADER_H

#include <stddef.h>

/**
 * Zeichnet ein Objekt auf den Bildschirm an der aktuellen Cursor-Position
 * @param charArray Zeiger auf das Character Array des Objekts
 * @param objectWidth Breite des Objekts in Zeichen
 * @param objectHeight Höhe des Objekts in Zeilen
 */
void draw_object(const char* charArray, unsigned char objectWidth, unsigned char objectHeight);

#endif

