#include <nds.h>	// Main NDS equates
#include "battle.h"
#include "utilities.h"
#include "player.h"

// equates
#include "constants.h"

// sprites
#include "battles.h"

void startBattle() {
	int i;
	dmaCopy(battlesPal, SPRITE_PALETTE, battlesPalLen);
	
	for(i = 0; i < 3; i++) {
		b_player[i].id = 0;		// 0 = first player (for now the filler sprites i've drawn
		b_player[i].action = A_STAND;
		b_player[i].frame = 0;	// animation counter
		b_player[i].x = 180;
		b_player[i].y = i*35+20;
	}
	bool running = true;
	while(running) {
		delay(1);
		drawPlayers();
		scanKeys();
		if(keysDown()&(KEY_A|KEY_B))
			running = false;
		if(keysDown()&KEY_DOWN)
			b_player[0].action = A_KICK_L;
		if(keysDown()&KEY_UP)
			b_player[0].action = A_KICK_H;
		if(keysDown()&KEY_RIGHT)
			b_player[0].action = A_PUNCH_R;
		if(keysDown()&KEY_LEFT)
			b_player[0].action = A_PUNCH_L;

	}
}

#define SPRITE_ENTRY_SIZE 32*32*2
#define SPRITES_PER_ACTION 2

void drawPlayers() {
	int i;
	for(i = 0; i < party.numMembers; i++) {
		int frame_number = (b_player[i].frame++/(ANIMATION_SPEED*4))%2;

		// find player's offset in sprite data and copy it to SPRITE_GFX
		int frame = frame_number + b_player[i].action * SPRITES_PER_ACTION;
		u8 *offset = (u8 *)battlesTiles + SPRITE_ENTRY_SIZE*(b_player[i].id) + frame * 32*32; // each frame = 32x32
		dmaCopy(offset, SPRITE_GFX+(32*32/2)*i, 32*32);	// copy sprite to OAM

		// set up sprite information/update x and y coordinates
		oamMain.oamMemory[i].attribute[0] = ATTR0_NORMAL | ATTR0_TYPE_NORMAL | ATTR0_COLOR_256 | ATTR0_SQUARE	| OBJ_Y(b_player[i].y);
		oamMain.oamMemory[i].attribute[1] = ATTR1_SIZE_32 | OBJ_X(b_player[i].x);

		oamMain.oamMemory[i].gfxIndex = i*4;	// set npc's sprite id
		oamMain.oamMemory[i].isHidden = false;
	}
	oamUpdate(&oamMain);
}
