#include <nds.h>
#include "player.h"
#include "objects.h"
#include "npcs.h"
#include "tilemap.h"

// sprite data
#include "character.h"

#define FRAMES_PER_ANIMATION 3
#define SPRITE_SIZE 16*32
#define SPRITE_ENTRY_SIZE 4*3*SPRITE_SIZE

extern NPC npcs[];
extern map_t Level;
extern int *npc_list[];

void loadNPCs(int map) {
	int i=0;
	int *map_n = (int *)npc_list[map];
	while(map_n[i] != -1) {
		npcs[i/4].x			= map_n[i];
		npcs[i/4].y			= map_n[i+1];
		npcs[i/4].string_id	= map_n[i+2];
		npcs[i/4].sprite_id	= map_n[i+3];
		npcs[i/4].active		= true;
		i += 4;
	}
	Level.numNPCs = i/4;
}

void animateNPCs(Drawable *player) {
	int i;
	for(i = 0; i < Level.numNPCs; i++) {
		int frame_number = npcs[i].anim_frame/ANIMATION_SPEED;

		if(frame_number > 1)
			frame_number = (frame_number%2)*2;

		int frame = frame_number + npcs[i].direction * FRAMES_PER_ANIMATION;
		u8 *offset = (u8 *)characterTiles + SPRITE_ENTRY_SIZE*(npcs[i].sprite_id) + frame * 16*32; // each frame = 16x32
	//	dmaCopy(offset, SPRITE_GFX+SPRITE_SIZE, 16*32); // 16x32
		dmaCopy(offset, SPRITE_GFX+16*32/2*(i+1), 16*32); // 16x32

		int nx = npcs[i].x-Level.x;
		int ny = npcs[i].y-Level.y;

		oamMain.oamMemory[i+1].attribute[0] = ATTR0_NORMAL | ATTR0_TYPE_NORMAL | ATTR0_COLOR_256 | ATTR0_TALL	| OBJ_Y(ny);
		oamMain.oamMemory[i+1].attribute[1] = ATTR1_SIZE_32 | OBJ_X(nx);

		oamMain.oamMemory[i+1].gfxIndex = i*2+2;	// set npc's sprite id

		if(nx < -16 || nx > 256 || ny < -16 || ny > 200)
			oamMain.oamMemory[i+1].isHidden = true;

		if(npcs[i].y > player->y)
			oamMain.oamMemory[i+1].priority = 1;	// set player's sprite priority to 1
		else
			oamMain.oamMemory[i+1].priority = 2;	// set player's sprite priority to 1
	}
	oamUpdate(&oamMain);
}
