#ifndef TEXT_H
#define TEXT_H

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

void putString(int x, int y, int w, const char *text, speed flag);
int stringLength(const char *text);
void drawBox(int x, int y, int w, int h);
void drawTextBox(int x, int y, int w, int h, const char *text, speed flag);
void delTextBox(int x, int y, int w, int h);

#endif // TEXT_H
