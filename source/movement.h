#ifndef MOVEMENT_H
#define MOVEMENT_H

void animatePC(Drawable *player);
void initPC(Drawable *player, u8 *gfx);
void movePlayer();
bool isPassable(int x, int y, int pdir, int ndir, int speed);

#endif /* MOVEMENT_H */
