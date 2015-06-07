#ifndef MENUS_H
#define MENUS_H

#include "objects.h"

typedef struct {
    int x;
    int y;
} MenuChoice;

int drawMenu(int x, int y, int w, char *text);

#endif // MENUS_H
