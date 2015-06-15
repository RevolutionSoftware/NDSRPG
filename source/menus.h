#ifndef MENUS_H
#define MENUS_H

#include "objects.h"

typedef struct {
    int x;
    int y;
    int w;
    int h;
    int counter;
} Box;

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


int drawMenu(int x, int y, int w, int h, char *text, int defValue);
void menuNewGame();
void menuMain();

void menuItems();
void dispItems();
int selectCharacter(int itemPos, int selected);

void menuEquip();
void viewEquip();
void chooseEquip(int pId, int eType);
void drawEquip(int offset, int pId, int eType);

void menuStats();

void menuStore(int id);
void buyItem(int storeId, int selected);
void drawStore(int storeId,int start,int selected);

#endif // MENUS_H
