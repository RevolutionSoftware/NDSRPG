#ifndef MOVEMENT_H
#define MOVEMENT_H

void animatePC(Drawable *player);
void initPC(Drawable *player, u8 *gfx);
void movePlayer(int *x, int *y, int direction, int speed);

#endif /* MOVEMENT_H */
