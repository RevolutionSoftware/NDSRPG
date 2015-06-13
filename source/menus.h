#ifndef MENUS_H
#define MENUS_H

#include "objects.h"

typedef struct {
    int x;
    int y;
} MenuChoice;

typedef struct {
    char *hello;
    char *goodbye;
    int *items;
    int numItems;
} Store;


int drawMenu(int x, int y, int w, char *text, int defValue);
void menuMain();
void menuStats();
void menuStore(int id);
void drawStore(int storeId,int start,int selected);

#endif // MENUS_H
