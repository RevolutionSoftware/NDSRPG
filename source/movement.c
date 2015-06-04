/* File: movement.c
 * Authors: Chickendude, add
 * Description: Movement of characters and objects */
#include <nds.h>
#include "player.h"
#include "objects.h"
#include "movement.h"

#define FRAMES_PER_ANIMATION 3

void animatePC(Drawable *player)
{
	int frame_number = player->anim_frame/ANIMATION_SPEED;
	// animation frames should be 01020102 etc.
	//							  01230123
	if(frame_number > 1)
		frame_number = (frame_number%2)*2;

    int frame = frame_number + player->state * FRAMES_PER_ANIMATION;
    u8 *offset = player->frame_gfx + frame * 32*32; // 32 * 32

    dmaCopy(offset, player->sprite_gfx_mem, 32*32); // 32 * 32
}

void initPC(Drawable *player, u8 *gfx)
{
    player->sprite_gfx_mem = oamAllocateGfx(&oamMain, SpriteSize_32x32, SpriteColorFormat_256Color);

    player->frame_gfx = (u8 *)gfx;
}
#undef FRAMES_PER_ANIMATION
