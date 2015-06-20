#include <nds.h>	// Main NDS equates
#include "battle.h"
#include "utilities.h"
#include "player.h"

// equates
#include "constants.h"

// sprites
#include "battlePlayer.h"
//#include "battleEnemy.h"
//#include "battle8x8.h"

void startBattle() {
	int i;
	dmaCopy(battlePlayerPal, SPRITE_PALETTE, battlePlayerPalLen);
	
	loadEnemies();
	
	for(i = 0; i < 3; i++) {
		b_player[i].id = 0;		// 0 = first player (for now the filler sprites i've drawn
		b_player[i].state = A_STAND;
		b_player[i].frame = 0;	// animation counter
		b_player[i].x = 180;
		b_player[i].y = i*35+20;
	}
	bool running = true;
	while(running) {
		delay(1);
		drawPlayers();
		drawEnemies();
		scanKeys();
		if(keysDown()&(KEY_A|KEY_B))
			running = false;
		if(keysDown()&KEY_DOWN)
			b_player[0].state = A_KICK_L;
		else if(keysDown()&KEY_UP)
			b_player[0].state = A_KICK_H;
		else if(keysDown()&KEY_RIGHT)
			b_player[0].state = A_PUNCH_R;
		else if(keysDown()&KEY_LEFT)
			b_player[0].state = A_PUNCH_L;
		else
			b_player[0].state = A_STAND;

	}
}

void loadEnemies() {
	enemy.numEnemies = randNum(2)+2;
	int i;
	for(i=0;i<enemy.numEnemies;i++) {
		enemy.list[i].x = 10;
		enemy.list[i].y = i*40+10;
		enemy.list[i].id = 0;
		enemy.list[i].frame = 0;
		enemy.list[i].hp = 10+i;
	}
}

#define SPRITE_ENTRY_SIZE 32*32*2
#define SPRITES_PER_ACTION 2

void drawPlayers() {
	int i;
	for(i = 0; i < party.numMembers; i++) {
		int frame_number = (b_player[i].frame++/(ANIMATION_SPEED*4))%2;

		// find player's offset in sprite data and copy it to SPRITE_GFX
		int frame = frame_number + b_player[i].state * SPRITES_PER_ACTION;
		u8 *offset = (u8 *)battlePlayerTiles + SPRITE_ENTRY_SIZE*(b_player[i].id) + frame * 32*32; // each frame = 32x32
		dmaCopy(offset, SPRITE_GFX+(32*32/2)*i, 32*32);	// copy sprite to OAM

		// set up sprite information/update x and y coordinates
		oamMain.oamMemory[i].attribute[0] = ATTR0_NORMAL | ATTR0_TYPE_NORMAL | ATTR0_COLOR_256 | ATTR0_SQUARE	| OBJ_Y(b_player[i].y);
		oamMain.oamMemory[i].attribute[1] = ATTR1_SIZE_32 | OBJ_X(b_player[i].x);

		oamMain.oamMemory[i].gfxIndex = i*4;	// set npc's sprite id
		oamMain.oamMemory[i].isHidden = false;
	}
	oamUpdate(&oamMain);
}

//2 sets of 6 sprites, 2 frames each, each frame is 32*32
#define ENEMY_SPRITE_OFFSET 2*6*2*32*32

void drawEnemies() {
	int i;
	for(i=0; i<enemy.numEnemies; i++) {
		int frame_number = (enemy.list[i].frame++/(ANIMATION_SPEED*4))%2;

		// find player's offset in sprite data and copy it to SPRITE_GFX
		int frame = frame_number + enemy.list[i].state * SPRITES_PER_ACTION;
		u8 *offset = (u8 *)battlePlayerTiles + ENEMY_SPRITE_OFFSET + enemy.list[i].id*SPRITE_ENTRY_SIZE + frame*32*32; // each frame = 32x32
		dmaCopy(offset, SPRITE_GFX+(i+3)*(32*32/2), 32*32);	// copy sprite to OAM

		// set up sprite information/update x and y coordinates
		oamMain.oamMemory[i+3].attribute[0] = ATTR0_NORMAL | ATTR0_TYPE_NORMAL | ATTR0_COLOR_256 | ATTR0_SQUARE	| OBJ_Y(enemy.list[i].y);
		oamMain.oamMemory[i+3].attribute[1] = ATTR1_SIZE_32 | OBJ_X(enemy.list[i].x);

		oamMain.oamMemory[i+3].gfxIndex = (i+3)*4;	// set npc's sprite id
		oamMain.oamMemory[i+3].isHidden = false;
	}
	oamUpdate(&oamMain);
}
