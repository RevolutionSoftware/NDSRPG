#ifndef TEXT_H
#define TEXT_H

/**************
 * \1 marks a menu choie
 * \2 displays character stats
 ***/

#define S_NAME "\x00"
#define S_LVL "\x01"
#define S_EXP "\x02"
#define S_HP_MAX "\x03"
#define S_HP "\x04"
#define S_STR "\x05"
#define S_DEF "\x06"
#define S_AGI "\x07"
#define S_WID "\x08"
#define S_AID "\x09"
#define S_STATE "\x0A"

#define STAT(stat) "\2" stat


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

//void putString(int x, int y, int w, const char *text, e_speed flag);
void putString(int x, int y, int w, e_speed flag, const char *text, ...);
void putStat(int *x, int *y, e_speed flag, int statId);
void putInt(int *x, int y, e_speed flag, int num);
void putChar(int *x, int y, e_speed flag, char c);
int stringHeight(const char *text);
void drawBox(int x, int y, int w, int h);
void drawTextBox(int x, int y, int w, int h, const char *text, e_speed flag);
void delTextBox(int x, int y, int w, int h);
void clrSubScreen();

#endif // TEXT_H
