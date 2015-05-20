#ifndef MOVEMENT_H
#define MOVEMENT_H

enum SpriteState {
    W_UP = 0,
    W_RIGHT = 1,
    W_DOWN = 2,
    W_LEFT = 3
};

void animate_PC(PC_t *sprite);

void init_PC(PC_t *sprite, u8 *gfx);

#endif /* MOVEMENT_H */
