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
				case 0:					// [up] / fight!
					if(!inputAttack(i))	// will return 0 if user cancelled attack
						i--;
					break;
				case 1:
					break;
				case 2:
					break;
				case 3:					// [down] / flee!
					running = false;
					i = party.numMembers;
				}
		}
		if(running) {
			// now load enemy attacks
		
			// then order by speed
		
			// then attack!
			for(i=0; i<party.numMembers; i++) {
				playerAttack(i);
			}
		}
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
	int atkBarLen = party.member[pId].atkBarLen;
	int i;
	// clear the attack list
	for(i = 0; i <= atkBarLen/7; i++) {
		b_player[pId].attackList[i] = -1;
	}
	clrSubScreen();		// clear the screen, display player's name, and draw empty attack bar
	putString(40,5,255,1,D_NONE,party.member[pId].name);
	
	drawAttackBar(atkBarLen);

	// receive attack bar input
	int atkBarPos = 0;
	bool running = true;
	int numAtk = 0;
	while(running) {
		delay(1);
		drawPlayers();	// keep animating players and enemies
		drawEnemies();
		scanKeys();
		int keys = keysDown();

		if(atkBarPos+letterWidth(AU) < atkBarLen) {
			if(keys&KEY_UP) {
				b_player[pId].attackList[numAtk++] = A_KICK_H;
				atkBarPos = putCharMask(18+atkBarPos,104,AU)-18;
			}
			else if(keys&KEY_LEFT) {
				b_player[pId].attackList[numAtk++] = A_PUNCH_L;
				atkBarPos = putCharMask(18+atkBarPos,104,AL)-18;
			}
			else if(keys&KEY_RIGHT) {
				b_player[pId].attackList[numAtk++] = A_PUNCH_R;
				atkBarPos = putCharMask(18+atkBarPos,104,AR)-18;
			}
			else if(keys&KEY_DOWN) {
				b_player[pId].attackList[numAtk++] = A_KICK_L;
				atkBarPos = putCharMask(18+atkBarPos,104,AD)-18;
			}
		}
		// if B was pressed, remove attacks/return to previous screen if no attacks were input
		if(keys&KEY_B) {
			if(atkBarPos == 0)			// if no attacks, return
				running = false;
			drawAttackBar(atkBarLen);	// redraw the attack bar (erasing arrow sprites)
			atkBarPos = 0;				// set the sprite pointer to start of attack bar
			numAtk = 0;					// restart the attack counter
			// remove attacks from the attack list
			for(i = 0; i < numAtk; i++)
				b_player[pId].attackList[i] = -1;
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

void playerAttack(int pId) {
	Damage atkDmg[20] = {0,0};
	int target = b_player[pId].target;
	b_player[pId].state = A_WALK;	// put player in walking state
	target = 1;
	int startX = b_player[pId].x;
	int startY = b_player[pId].y;
	int tX = enemy.list[target].x;
	int tY = enemy.list[target].y*10;
	int pX = b_player[pId].x;
	int pY = b_player[pId].y*10;
	int dY;
	// make player run over to the target enemy
	while(b_player[pId].x > tX+30) {
		delay(1);
		drawPlayers();
		b_player[pId].frame++;		// make player run "faster"
		drawEnemies();
		dY = (tY - pY)/(pX-(tX+30));
		pY += dY*2;
		pX-=2;
		b_player[pId].y = pY/10;
		b_player[pId].x = pX;
	}
	bool running = true;
	int atkNum = -1;
	b_player[pId].frame = -2;
	// run through list of attacks
	while(running) {
		delay(1);
		drawPlayers();
		b_player[pId].frame++;
		drawEnemies();
		if(b_player[pId].frame%(ANIMATION_SPEED*4*2) == 0) {
			b_player[pId].state = b_player[pId].attackList[++atkNum];
			if(b_player[pId].attackList[atkNum] == -1)
				running = false;
		}
		if(b_player[pId].state == b_player[pId].attackList[atkNum] && (b_player[pId].frame/(ANIMATION_SPEED*4))%2 == 1) {
			enemy.list[target].state = 1;
			enemy.list[target].frame = ANIMATION_SPEED*3;
			atkDmg[atkNum].dmg = getAtk(pId);
			atkDmg[atkNum].counter = 16;
		}
	}
	b_player[pId].state = A_STAND;
	b_player[pId].x = startX;		// restore player to orig position
	b_player[pId].y = startY;
	enemy.list[target].state = 0;
}
