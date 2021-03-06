#ifndef OBJECTS_H
#define OBJECTS_H

// We can make the Player-Character datatype better soon enough.
typedef struct {
    int x;
    int y;

    u16 *sprite_gfx_mem;
    u8  *frame_gfx;

    int state;
    int anim_frame;
}Drawable;

SpriteEntry* initSprite(int spriteid, u8 *gfx, int len);


#endif /* OBJECTS_H */
