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

void drawBox(int, int, alignment);
void putString(const char *text, speed flag);
int stringLength(const char *text);

#endif // TEXT_H
