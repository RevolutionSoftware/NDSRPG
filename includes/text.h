#ifndef TEXT_H
#define TEXT_H

#include "menus.h"


/**************
 * \1 marks a menu choie
 * \2 displays character stats
 ***/


#define NEWX "\2"


typedef enum {
    D_NONE,
    D_FAST,
    D_SLOW
} e_speed;

typedef struct {
    int x;
    int y;
    int layer;
} coord_t;

int letterWidth(int letter);
void putString(int x, int y, int w, int h, e_speed flag, const char *text, ...);
int stringLength(const char *text);
int stringWidth(const char *text);
void putStat(int *x, int *y, e_speed flag, int statId);
int putInt(int x, int y, e_speed flag, int num);
int putChar(int x, int y, e_speed flag, char c);
int putCharMask(int x, int y, char c);
int stringHeight(const char *text);
int intWidth(int num);
// boxes
void drawBox(int x, int y, int w, int h);
void drawBoxType(int x, int y, int w, int h, int type, int bg);
void drawBoxes(Box boxes[], int numBoxes, int selected);
void drawTextBox(int x, int y, int w, int h, int type, e_speed flag, const char *text);
// erasing stuff
void delText(int x, int y, int w, int h);
void delTextBox(int x, int y, int w, int h);
void clrSubScreen();

#endif // TEXT_H
