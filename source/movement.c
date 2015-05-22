/* File: movement.c
 * Authors: Chickendude, add
 * Description: Movement of characters and objects */
#include <nds.h>
#include "player.h"
#include "objects.h"
#include "movement.h"

#define FRAMES_PER_ANIMATION 3

void animate_PC(PC_t *sprite)
{
    int frame = sprite->anim_frame/ANIMATION_SPEED + sprite->state * FRAMES_PER_ANIMATION;
    u8 *offset = sprite->frame_gfx + frame * 32*32; // 32 * 32

    dmaCopy(offset, sprite->sprite_gfx_mem, 32*32); // 32 * 32
}

void init_PC(PC_t *sprite, u8 *gfx)
{
    sprite->sprite_gfx_mem = oamAllocateGfx(&oamMain, SpriteSize_32x32, SpriteColorFormat_256Color);

    sprite->frame_gfx = (u8 *)gfx;
}
#undef FRAMES_PER_ANIMATION
