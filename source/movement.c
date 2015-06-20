/* File: movement.c
 * Authors: Chickendude, add
 * Description: Movement of characters and objects */
#include <nds.h>
#include "constants.h"
#include "player.h"
#include "objects.h"
#include "movement.h"
#include "tilemap.h"
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
	int keys = keysHeld();

	if (keys & KEY_RIGHT)
	{
		player.state = W_RIGHT;
		if(isPassable(player.x,player.y,player.state,-1,SPEED))
			player.x+=SPEED;
		// Make sure map isn't at edge of screen
		if ((player.x-Level.x > SCREEN_RIGHT/2 - PLAYER_WIDTH/2) && Level.x < Level.w*16-16*16)
			Level.x+=SPEED;
	}
	if (keys & KEY_LEFT)
	{
		player.state = W_LEFT;
		if(isPassable(player.x,player.y,player.state,-1,SPEED))
			player.x-=SPEED;
		if ((player.x-Level.x < SCREEN_RIGHT/2 - PLAYER_WIDTH/2) && Level.x>0)
			Level.x-=SPEED;
	}
	if (keys & KEY_DOWN)		// w*tile_height - tile_height*tiles_per_column
	{
		player.state = W_DOWN;
		if(isPassable(player.x,player.y,player.state,-1,SPEED))
			player.y+=SPEED;
		if ((player.y-Level.y > SCREEN_BOTTOM/2 - PLAYER_HEIGHT/2) && Level.y < Level.h*16-12*16)
			Level.y+=SPEED;
	}
	if (keys & KEY_UP)
	{
		player.state = W_UP;
		if(isPassable(player.x,player.y,player.state,-1,SPEED))
			player.y-=SPEED;
		if ((player.y-Level.y < SCREEN_BOTTOM/2 - PLAYER_HEIGHT/2) && Level.y > 0)
			Level.y-=SPEED;
	}
	if ((keys&KEY_LEFT) | (keys&KEY_RIGHT) | (keys&KEY_DOWN) | (keys&KEY_UP)) {
		// top bg
		REG_BG0HOFS = (Level.x)%16;
		REG_BG0VOFS = (Level.y)%16;
		// bottom bg
		REG_BG1HOFS = (Level.x)%16;
		REG_BG1VOFS = (Level.y)%16;
		player.anim_frame++;
		player.anim_frame %= (FRAMES_PER_ANIMATION+1)*ANIMATION_SPEED;
		checkTile(T_MOTION);
	} else
		player.anim_frame = 0;	// reset animation when not moving
}

bool isPassable(int x, int y, int pdir, int ndir, int speed) {
	int direction = pdir;
	if(pdir == -1)
		direction = ndir;
	bool passable = true;
	
	// check if player will collide with an NPC
	if(ndir == -1) {
		int i;
		for(i = 0; i < Level.numNPCs; i++) {
			if(NPCCollision(i,pdir,-1))
				passable = false;
		}
	}
	// if not, check if player will collide with a tile
	if(passable) {
		if(direction == W_RIGHT) {
			int mapOffset = (y + PLAYER_Y_OVERLAP)/16*Level.w+(x + PLAYER_WIDTH + SPEED)/16;
			int tileId = Level.map[mapOffset];
			int tileId2 = tileId;
			if ((y % 16) >= 9)
				tileId2 = Level.map[mapOffset+Level.w];	// move down one row in map
			
			// Make sure player isn't at edge of screen
			if(!(x < Level.w*16-PLAYER_WIDTH))
				passable = false;
			if(!(Level.tiles[tileId*3].isPassable && Level.tiles[tileId2*3].isPassable))
				passable = false;
		}
		else if(direction == W_LEFT) {
			int mapOffset = (y+16)/16*Level.w+(x-SPEED)/16;
			int tileId = Level.map[mapOffset];
			int tileId2 = tileId;
			if (y % 16 >= 9)
				tileId2 = Level.map[mapOffset+Level.w];	// move down one row in map
			if (x <= 0)
				passable = false;
			if(!(Level.tiles[tileId*3].isPassable && Level.tiles[tileId2*3].isPassable))
				passable = false;
		}
		else if(direction == W_DOWN) {
			int mapOffset = (y+PLAYER_HEIGHT+SPEED)/16*Level.w+x/16;
			int tileId = Level.map[mapOffset];
			int tileId2 = tileId;
			if (x % 16 != 0)
				tileId2 = Level.map[mapOffset+1];
			if(!(y < Level.h*16 - PLAYER_HEIGHT))
				passable = false;
			if(!(Level.tiles[tileId*3].isPassable && Level.tiles[tileId2*3].isPassable))
				passable = false;
		}
		else if(direction == W_UP) {
			int mapOffset = (y+16-SPEED)/16*Level.w+x/16;
			int tileId = Level.map[mapOffset];
			int tileId2 = tileId;
			if(x % 16 != 0)
				tileId2 = Level.map[mapOffset+1];
			if(y <= 0)
				passable = false;
			if(!(Level.tiles[tileId*3].isPassable && Level.tiles[tileId2*3].isPassable))
				passable = false;
		}
	}
	return passable;
}

#undef FRAMES_PER_ANIMATION
