/* File: movement.c
 * Authors: Chickendude, add
 * Description: Movement of characters and objects */
#include <nds.h>
#include "player.h"
#include "objects.h"
#include "movement.h"
#include "tilemap.h"
#include "constants.h"
#include "npcs.h"

#define FRAMES_PER_ANIMATION 3
#define SPRITE_SIZE 16*32
#define SPRITE_ENTRY_SIZE 4*3*SPRITE_SIZE

extern map_t Level;
extern Drawable player;

void animatePC(Drawable *player)
{
	int frame_number = player->anim_frame/ANIMATION_SPEED;
	// animation frames should be 01020102 etc.
	//							  01230123
	if(frame_number > 1)
		frame_number = (frame_number%2)*2;

	int frame = frame_number + player->state * FRAMES_PER_ANIMATION;
	u8 *offset = player->frame_gfx + frame * 16*32; // each frame = 16x32

	dmaCopy(offset, player->sprite_gfx_mem, 16*32); // 16x32
}

void initPC(Drawable *player, u8 *gfx)
{
	oamMain.oamMemory[0].priority = 2;		// set player's sprite priority to 2
	player->sprite_gfx_mem = SPRITE_GFX;	// oamAllocateGfx(&oamMain, SpriteSize_32x32, SpriteColorFormat_256Color);
	player->frame_gfx = (u8 *)gfx;
}

void movePlayer() {
	// check if player will collide with an NPC
	bool hitNPC = false;
	int i;
	for(i = 0; i < Level.numNPCs; i++) {
		if(NPCCollision(i))
			hitNPC = true;
	}
	// if not, check if player will collide with a tile
	if(!hitNPC) {
		if(player.state == W_RIGHT) {
			int mapOffset = (player.y + PLAYER_Y_OVERLAP)/16*Level.w+(player.x + PLAYER_WIDTH + SPEED)/16;
			int tileId = Level.map[mapOffset];
			int tileId2 = tileId;
			if ((player.y % 16) >= 9)
				tileId2 = Level.map[mapOffset+Level.w];	// move down one row in map
			// Make sure player isn't at edge of screen
			if (player.x < Level.w*16-PLAYER_WIDTH)
				if(Level.tiles[tileId*3].isPassable && Level.tiles[tileId2*3].isPassable)
					player.x+=SPEED;
			// Make sure map isn't at edge of screen
			if ((player.x-Level.x > SCREEN_RIGHT/2 - PLAYER_WIDTH/2) && Level.x < Level.w*16-16*16)
				Level.x+=SPEED;
		}
		else if(player.state == W_LEFT) {
			int mapOffset = (player.y+16)/16*Level.w+(player.x-SPEED)/16;
			int tileId = Level.map[mapOffset];
			int tileId2 = tileId;
			if (player.y % 16 >= 9)
				tileId2 = Level.map[mapOffset+Level.w];	// move down one row in map
			if (player.x > 0)
				if(Level.tiles[tileId*3].isPassable && Level.tiles[tileId2*3].isPassable)
					player.x-=SPEED;
			if ((player.x-Level.x < SCREEN_RIGHT/2 - PLAYER_WIDTH/2) && Level.x>0)
				Level.x-=SPEED;
		}
		else if(player.state == W_DOWN) {
			int mapOffset = (player.y+PLAYER_HEIGHT+SPEED)/16*Level.w+player.x/16;
			int tileId = Level.map[mapOffset];
			int tileId2 = tileId;
			if (player.x % 16 != 0)
				tileId2 = Level.map[mapOffset+1];
			if (player.y < Level.h*16 - PLAYER_HEIGHT)
				if(Level.tiles[tileId*3].isPassable && Level.tiles[tileId2*3].isPassable)
					player.y+=SPEED;
			if ((player.y-Level.y > SCREEN_BOTTOM/2 - PLAYER_HEIGHT/2) && Level.y < Level.h*16-12*16)
				Level.y+=SPEED;
		}
		else if(player.state == W_UP) {
			int mapOffset = (player.y+16-SPEED)/16*Level.w+player.x/16;
			int tileId = Level.map[mapOffset];
			int tileId2 = tileId;
			if (player.x % 16 != 0)
				tileId2 = Level.map[mapOffset+1];
			if (player.y > 0)
				if(Level.tiles[tileId*3].isPassable && Level.tiles[tileId2*3].isPassable)
					player.y -= SPEED;
			if ((player.y-Level.y < SCREEN_BOTTOM/2 - PLAYER_HEIGHT/2) && Level.y > 0)
				Level.y-=SPEED;
		}
	}
}

#undef FRAMES_PER_ANIMATION
