#ifndef TEXT_H
#define TEXT_H

typedef enum {
    NONE = 0,
    FAST = 1,
    SLOW = 2,
};

typedef struct {
    int x;
    int y;
    int layer;
} coord_t;

void drawBox(int, int, coord_t);
void putString(const char *text, unsigned int flag);
int stringLength(const char *text);

#endif // TEXT_H
