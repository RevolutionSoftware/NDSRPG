#include <nds.h>
#include "player.h"
#include "objects.h"
#include "npcs.h"
#include "tilemap.h"
#include "constants.h"
#include "movement.h"
#include "utilities.h"

#include "text.h"

// sprite data
#include "character.h"

#define FRAMES_PER_ANIMATION 3
#define SPRITE_SIZE 16*32
#define SPRITE_ENTRY_SIZE 4*3*SPRITE_SIZE

extern NPC npcs[];
extern map_t Level;
extern int *npc_list[];
extern int *npc_paths[];
extern Drawable player;

void loadNPCs(int map) {
	int i=0;
	int *map_n = (int *)npc_list[map];
	while(map_n[i] != -1) {
		npcs[i/NPC_ENTRY].startx	= map_n[i];
		npcs[i/NPC_ENTRY].starty	= map_n[i+1];
		npcs[i/NPC_ENTRY].x			= map_n[i];
		npcs[i/NPC_ENTRY].y			= map_n[i+1];
		npcs[i/NPC_ENTRY].x1		= map_n[i+2];
		npcs[i/NPC_ENTRY].y1		= map_n[i+3];
		npcs[i/NPC_ENTRY].x2		= map_n[i+4];
		npcs[i/NPC_ENTRY].y2		= map_n[i+5];
		npcs[i/NPC_ENTRY].string_id	= map_n[i+6];
		npcs[i/NPC_ENTRY].sprite_id	= map_n[i+7];
		npcs[i/NPC_ENTRY].direction	= map_n[i+8];
		npcs[i/NPC_ENTRY].path.set	= map_n[i+9];
		npcs[i/NPC_ENTRY].path.pos	= -2;
		npcs[i/NPC_ENTRY].path.ctr	= 0;
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

void moveNPCs() {
	int id;
	for(id = 0; id < Level.numNPCs; id++) {
		int pId = npcs[id].path.set;
		int *path = npc_paths[pId];
		int pos = npcs[id].path.pos;

		// if ctr = 0, load the next path
		if(npcs[id].path.ctr == 0) {
			pos+=2;
			if(path[pos] == -1)
				pos = 0;
			npcs[id].path.ctr = path[pos+1];
			npcs[id].path.action = path[pos];
		}
		npcs[id].path.pos = pos;

		switch(npcs[id].path.action) {
			case 0:			// walk right
				npcs[id].direction = W_RIGHT;
				if(NPCCollision(id,-1,npcs[id].direction))
					break;
				if(isPassable(npcs[id].x,npcs[id].y,-1,npcs[id].direction,1)) {
					npcs[id].x++;
					npcs[id].path.ctr--;
				} else
					npcs[id].path.ctr = 0;
				break;
			case 1:			// walk left
				npcs[id].direction = W_LEFT;
				if(NPCCollision(id,-1,npcs[id].direction))
					break;
				if(isPassable(npcs[id].x,npcs[id].y,-1,npcs[id].direction,1)) {
					npcs[id].x--;
					npcs[id].path.ctr--;
				} else
					npcs[id].path.ctr = 0;
				break;
			case 2:			// walk down
				npcs[id].direction = W_DOWN;
				if(NPCCollision(id,-1,npcs[id].direction))
					break;
				if(isPassable(npcs[id].x,npcs[id].y,-1,npcs[id].direction,1)) {
					npcs[id].y++;
					npcs[id].path.ctr--;
				} else
					npcs[id].path.ctr = 0;
				break;
			case 3:			// walk up
				npcs[id].direction = W_UP;
				if(NPCCollision(id,-1,npcs[id].direction))
					break;
				if(isPassable(npcs[id].x,npcs[id].y,-1,npcs[id].direction,1)) {
					npcs[id].y--;
					npcs[id].path.ctr--;
				} else
					npcs[id].path.ctr = 0;
				break;
			case 4:			// wait
				npcs[id].anim_frame = 0;
				npcs[id].path.ctr--;
				break;
			case 5:			// face a certain direction
				npcs[id].direction = path[pos+1];
				npcs[id].path.ctr = 0;
				break;
			case 6:			// random
				npcs[id].path.action = randNum(10) < 4 ? randNum(4) : 4;
				
				// make them turn around if they are supposed to walk out of their boundary
				if(	npcs[id].path.action != 4 &&
					((npcs[id].x == npcs[id].x1 && npcs[id].path.action == 1) ||
					(npcs[id].x == npcs[id].x2 && npcs[id].path.action == 0) ||
					(npcs[id].y == npcs[id].y1 && npcs[id].path.action == 3) ||
					(npcs[id].y == npcs[id].y2 && npcs[id].path.action == 2)))
						npcs[id].path.action -= (npcs[id].path.action%1)*2-1;
				npcs[id].path.ctr = 16*(randNum(3)+1);
				break;
		}

		if(npcs[id].x < npcs[id].x1) {
			npcs[id].x = npcs[id].x1;
			npcs[id].path.ctr = 0;
		}

		if(npcs[id].y < npcs[id].y1) {
			npcs[id].y = npcs[id].y1;
			npcs[id].path.ctr = 0;
		}

		if(npcs[id].x > npcs[id].x2) {
			npcs[id].x = npcs[id].x2;
			npcs[id].path.ctr = 0;
		}

		if(npcs[id].y > npcs[id].y2) {
			npcs[id].y = npcs[id].y2;
			npcs[id].path.ctr = 0;
		}
		
		npcs[id].anim_frame++;
		if(NPCCollision(id,-1,npcs[id].direction))
			npcs[id].anim_frame = 0;
		npcs[id].anim_frame %= (FRAMES_PER_ANIMATION+1)*ANIMATION_SPEED;
	}
}

bool NPCCollision(int id, int pdir, int ndir) {
	int pxoff=0;
	int pyoff=0;
	int nxoff=0;
	int nyoff=0;

	// if ndir is set, we are checking if the NPC will collide with the player
	if(ndir != -1) {
		if(ndir == W_LEFT)
			nxoff-=SPEED;
		if(ndir == W_RIGHT)
			nxoff+=SPEED;
		if(ndir == W_DOWN)
			nyoff+=SPEED;
		if(ndir == W_UP)
			nyoff-=SPEED;
	}

	// if pdir is set, we're checking if the player will collide with the NPC
	if(pdir != -1) {
		if(pdir == W_LEFT)
			pxoff-=SPEED;
		if(pdir == W_RIGHT)
			pxoff+=SPEED;
		if(pdir == W_DOWN)
			pyoff+=SPEED;
		if(pdir == W_UP)
			pyoff-=SPEED;
	}

	// the +3 lets you get a little closer to the NPC
	return (player.x + pxoff + 3 <= npcs[id].x + nxoff + PLAYER_WIDTH) && (player.x + pxoff + PLAYER_WIDTH >= npcs[id].x + nxoff +3) && (player.y + pyoff <= npcs[id].y + nyoff + 24 - PLAYER_Y_OVERLAP) && (player.y + pyoff + 24 - PLAYER_Y_OVERLAP >= npcs[id].y + nyoff);
}
