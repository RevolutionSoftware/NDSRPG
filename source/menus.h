#ifndef MENUS_H
#define MENUS_H

#include "objects.h"

void drawMenu(int x, int y, int w, int h, char *text);
int selectOption ();
void drawCursor(Drawable *cursor);
void initCursor(Drawable *cursor, u8 *gfx);

#endif // MENUS_H
