#include <nds.h>	// Main NDS equates
#include "battle.h"
#include "utilities.h"
#include "player.h"
#include "text.h"

// equates
#include "constants.h"

// sprites
#include "battlePlayer.h"
//#include "battleEnemy.h"
//#include "battle8x8.h"

// box that actions are put into
#define BL	'~'+1
#define BC	BL+1
#define BR	BC+1
// arrows that fill the box
#define AU	BR+1
#define AL	AU+1
#define AR	AL+1
#define AD	AR+1



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
		for(i=0; i<party.numMembers; i++) {
			int action = battleMenu(i);
			switch(action) {
				case 0:			// [up] / fight!
					if(!inputAttack(i))
						i--;
					break;
				case 1:
					break;
				case 2:
					break;
				case 3:
					running = false;
					i = party.numMembers;
				}
		}
		// now load enemy attacks
		// then order by speed
		// then attack!
	}
	for(i=0;i<10;i++)
		oamMain.oamMemory[i].isHidden = true;
	clrSubScreen();
}

int battleMenu(int pId) {
	putString(40,5,255,1,D_NONE,party.member[pId].name);
	putString(13*8+5,3*8,100,1,D_NONE,"Fight");
	putString(9*8+7,4*8,100,1,D_NONE,"Skill");
	putString(17*8+7,4*8,100,1,D_NONE,"Item");
	putString(13*8+8,5*8,100,1,D_NONE,"Flee");

	int selection = -1;
	while(selection < 0) {
		delay(1);
		scanKeys();
		int keys = keysHeld();

		drawBoxType(13,3,5,1,2+((keys&KEY_UP)>0),0);
		drawBoxType(9,4,5,1,2+((keys&KEY_LEFT)>0),1);
		drawBoxType(17,4,5,1,2+((keys&KEY_RIGHT)>0),1);
		drawBoxType(13,5,5,1,2+((keys&KEY_DOWN)>0),0);


		drawPlayers();
		drawEnemies();
		if(keysUp()&KEY_UP)
			selection++;
		else if(keysUp()&KEY_LEFT)
			selection = 1;
		else if(keysUp()&KEY_RIGHT)
			selection = 2;
		else if(keysUp()&KEY_DOWN)
			selection = 3;
	}
	return selection;
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

int inputAttack(int pId) {
	clrSubScreen();
	putString(40,5,255,1,D_NONE,party.member[pId].name);
	int atkBarLen = 41;
	drawAttackBar(atkBarLen);

	int atkBarPos = 0;
	bool running = true;
	while(running) {
		delay(1);
		drawPlayers();	// keep animating players and enemies
		drawEnemies();
		scanKeys();
		int keys = keysDown();

		if(atkBarPos+letterWidth(AU) < atkBarLen) {
			if(keys&KEY_UP) {
				atkBarPos = putCharMask(18+atkBarPos,104,AU)-18;
			}
			else if(keys&KEY_LEFT) {
				atkBarPos = putCharMask(18+atkBarPos,104,AL)-18;
			}
			else if(keys&KEY_RIGHT) {
				atkBarPos = putCharMask(18+atkBarPos,104,AR)-18;
			}
			else if(keys&KEY_DOWN) {
				atkBarPos = putCharMask(18+atkBarPos,104,AD)-18;
			}
		}
		if(keys&KEY_B) {
			if(atkBarPos == 0)
				running = false;
			drawAttackBar(atkBarLen);
			atkBarPos = 0;
		}
		else if(keys&KEY_A)
			if(atkBarPos > 0)
				running = false;
	}
	clrSubScreen();
	return atkBarPos;
}

void drawAttackBar(int length) {
	char atkBar[70] = {0};
	int i;
	atkBar[0] = BL;
	for(i=1;i<=length/2;i++) {
		atkBar[i] = BC;
	}
	atkBar[i] = BR;
	delText(10,100,length,8);
	putString(10,100,255,1,D_NONE,atkBar);
}
