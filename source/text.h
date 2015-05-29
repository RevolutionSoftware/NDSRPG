#ifndef TEXT_H
#define TEXT_H

typedef enum speed {
    D_NONE,
    D_FAST,
    D_SLOW
} speed;

typedef enum alignment {
    A_NONE,
    A_CENTER,
    A_BOTTOM
} alignment;

typedef struct {
    int x;
    int y;
    int layer;
} coord_t;

void putString(const char *text, speed flag);
int stringLength(const char *text);
void drawBox(int x, int y, int w, int h);
void delBox(int x, int y, int w, int h);

#endif // TEXT_H
