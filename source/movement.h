#ifndef MOVEMENT_H
#define MOVEMENT_H

enum SpriteState {
    W_UP = 0,
    W_RIGHT = 1,
    W_DOWN = 2,
    W_LEFT = 3
};

void animatePC(Drawable *player);
void initPC(Drawable *player, u8 *gfx);

#endif /* MOVEMENT_H */
