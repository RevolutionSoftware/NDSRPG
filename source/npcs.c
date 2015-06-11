#include <nds.h>
#include "player.h"
#include "objects.h"
#include "npcs.h"
#include "tilemap.h"
#include "constants.h"

// sprite data
#include "character.h"

#define FRAMES_PER_ANIMATION 3
#define SPRITE_SIZE 16*32
#define SPRITE_ENTRY_SIZE 4*3*SPRITE_SIZE

extern NPC npcs[];
extern map_t Level;
extern int *npc_list[];
extern Drawable player;

void loadNPCs(int map) {
	int i=0;
	int *map_n = (int *)npc_list[map];
	while(map_n[i] != -1) {
		npcs[i/NPC_ENTRY].x			= map_n[i];
		npcs[i/NPC_ENTRY].y			= map_n[i+1];
		npcs[i/NPC_ENTRY].string_id	= map_n[i+2];
		npcs[i/NPC_ENTRY].sprite_id	= map_n[i+3];
		npcs[i/NPC_ENTRY].direction	= map_n[i+4];
		npcs[i/NPC_ENTRY].active	= true;
		i += NPC_ENTRY;
	}
	Level.numNPCs = i/NPC_ENTRY;
}

void animateNPCs() {
	int i;
	for(i = 0; i < Level.numNPCs; i++) {
		int frame_number = npcs[i].anim_frame/ANIMATION_SPEED;

		if(frame_number > 1)
			frame_number = (frame_number%2)*2;

		int frame = frame_number + npcs[i].direction * FRAMES_PER_ANIMATION;
		u8 *offset = (u8 *)characterTiles + SPRITE_ENTRY_SIZE*(npcs[i].sprite_id) + frame * 16*32; // each frame = 16x32
		dmaCopy(offset, SPRITE_GFX+16*32/2*(i+1), 16*32); // copy sprite to OAM

		// NPC's position on screen
		int nx = npcs[i].x-Level.x;
		int ny = npcs[i].y-Level.y;

		// set up sprite information/update x and y coordinates
		oamMain.oamMemory[i+1].attribute[0] = ATTR0_NORMAL | ATTR0_TYPE_NORMAL | ATTR0_COLOR_256 | ATTR0_TALL	| OBJ_Y(ny);
		oamMain.oamMemory[i+1].attribute[1] = ATTR1_SIZE_32 | OBJ_X(nx);

		oamMain.oamMemory[i+1].gfxIndex = i*2+2;	// set npc's sprite id

		// check if NPC is on screen
		if(nx < -16 || nx > 256 || ny < -16 || ny > 200)
			oamMain.oamMemory[i+1].isHidden = true;

		// check if NPC should be drawn above or below the player
		if(npcs[i].y > player.y)
			oamMain.oamMemory[i+1].priority = 1;	// set player's sprite priority to 1
		else
			oamMain.oamMemory[i+1].priority = 2;	// set player's sprite priority to 1
	}
	oamUpdate(&oamMain);
}

#include "text.h"


bool NPCCollision(int id) {
	int xoff=0;
	int yoff=0;

	if(player.state == W_LEFT)
		xoff-=SPEED;
	if(player.state == W_RIGHT)
		xoff+=SPEED;
	if(player.state == W_DOWN)
		yoff+=SPEED;
	if(player.state == W_UP)
		yoff-=SPEED;

	// the +3 lets you get a little closer to the NPC
	return (player.x + xoff + 3 <= npcs[id].x + PLAYER_WIDTH) && (player.x + xoff + PLAYER_WIDTH >= npcs[id].x+3) && (player.y + yoff <= npcs[id].y + 24 - PLAYER_Y_OVERLAP) && (player.y + yoff + 24 - PLAYER_Y_OVERLAP >= npcs[id].y);
}
