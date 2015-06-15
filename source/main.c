/* File: main.c
 * Authors: Chickendude, add
 * Description: File containing main */

#define DEBUG



// Includes NDS
#include <nds.h>	// Main NDS equates
#include <stdio.h>	// For console stuff

#include "aux_macros.h"
#include "tilemap.h"
#include "objects.h"
#include "movement.h"
#include "text.h"
#include "menus.h"
#include "constants.h"
#include "utilities.h"
#include "player.h"
#include "weapons.h"
#include "armor.h"
#include "items.h"
#include "npcs.h"

// sprite data
#include "font.h"
#include "tiles.h"
#include "character.h"
#include "cursor.h"


#include "maps/maps.h"		// map data
#include "texts/texts.h"	// text data
#include "texts/menus.h"	// text for menus
#include "texts/weapons.h"
#include "texts/armor.h"
#include "texts/items.h"
#include "npcs/npcs.h"

#define FRAMES_PER_ANIMATION 3

// Equates
enum {
	M_BASE0 = 0,
	M_BASE1 = 1,
	M_BASE2 = 2,
	M_BASE3 = 3,

	T_BASE0 = 0,
	T_BASE1 = 1
};

NPC npcs[10];
map_t Level;
Drawable player;

int main(void) {
	int i;
	player = (Drawable) {16,32};
	Level = map_list[0];
	Level.x = 0;
	Level.y = 0;
	
	loadNPCs(0);
	
// setup party
	party.member[0] = (Character) {"add",1,0,260,60,10,19,13,0,1};
	party.member[1] = (Character) {"chickendude",1,0,50,40,16,15,3,1,0};
	party.member[2] = (Character) {"NanoWar",90,92860,1650,1045,168,135,83,3,2};

	party.member[0].active = true;
	party.member[1].active = true;
	party.member[2].active = true;
	
	party.numMembers = 3;
	
	party.gold = 133337;

	for(i = 0; i < 4; i++) {
		party.inventory[i].id	= i;
		party.inventory[i].amt	= randNum(2)+2;
	}

	for(i = 4; i < MAX_ITEMS; i++) {
		party.inventory[i].id	= -1;
		party.inventory[i].amt	= 0;
	}

	for(i = 0; i < 10; i++) {
		party.weapons[i]	= randNum(4);
	}

	for(i = 0; i < 10; i++) {
		party.armor[i]	= randNum(4);
	}


	/* NDS has nine memory banks, banks 0-4
	 *  Use mode 0. Mode 0 is for tilebased sprites, called "text" mode
	 * Other modes have options for rotation, scaling,
	 * and bitmap display. You have access to 4 backgrounds in mode 0 */
// background
	vramSetBankA(VRAM_A_MAIN_BG_0x06000000);
// main character/npcs
	vramSetBankB(VRAM_B_MAIN_SPRITE_0x06400000);
// text
//	vramSetBankC(VRAM_C_SUB_BG);
	vramSetBankH(VRAM_H_SUB_BG);

	REG_DISPCNT = MODE_0_2D | DISPLAY_SPR_ACTIVE | DISPLAY_SPR_1D | DISPLAY_SPR_1D_SIZE_256 | DISPLAY_BG0_ACTIVE | DISPLAY_BG1_ACTIVE;   // Set mode 0 in 2D mode and enable background 0
	REG_BG0CNT = REG_BG0CNT_DEFAULT;	// see constants.h
	REG_BG1CNT = REG_BG1CNT_DEFAULT;	// map base = 2kb, tile base = 16kb

// Font stuff for bottom screen
// Sub screen uses bg1, sub map base 0 and sub tile base 1.
	REG_DISPCNT_SUB = MODE_0_2D | DISPLAY_SPR_ACTIVE | DISPLAY_SPR_1D | DISPLAY_SPR_1D_SIZE_128 | DISPLAY_BG0_ACTIVE| DISPLAY_BG1_ACTIVE;   // bottom screen, use bg0 and bg1
	REG_BG0CNT_SUB = BG_PRIORITY_2 | BG_32x32 | BG_COLOR_16 | BG_MAP_BASE(0) | BG_TILE_BASE(1); // text
	REG_BG1CNT_SUB = BG_PRIORITY_3 | BG_32x32 | BG_COLOR_16 | BG_MAP_BASE(1) | BG_TILE_BASE(1); // textboxes



// Copy font data (sprites and palette)
	dmaCopy(fontTiles, BG_TILE_RAM_SUB(1), fontTilesLen);
	dmaCopy(fontPal, BG_PALETTE_SUB, fontPalLen);

// Sprite data
	for (i=0;i<128;i++) {
		oamMain.oamMemory[i].isHidden=true;
		oamSub.oamMemory[i].isHidden=true;
	}
	oamUpdate(&oamSub);
	oamUpdate(&oamMain);

	initPC(&player, (u8 *)characterTiles);
	dmaCopy(characterPal, SPRITE_PALETTE, 512);

	// load menu cursor palette
	dmaCopy(cursorPal, SPRITE_PALETTE_SUB, cursorPalLen);	// copy palette for sub engine (should only have one for the entire engine)

	// tile_ram is divided into blocks of 16kb, tileram(1) = tile_ram + 16kb
	dmaCopy(tilesTiles, BG_TILE_RAM(1), tilesTilesLen);
	dmaCopy(tilesPal, BG_PALETTE, 256*2);
// **********************************************************************************************

	REG_BG1HOFS = 0;
	REG_BG1VOFS = 0;

	// shift the text a few pixels down and to the right to not collide with the box border
	REG_BG0HOFS_SUB = -4;
	REG_BG0VOFS_SUB = -4;

//	menuNewGame();
//	clrSubScreen();

	while(1) {
		// Check for keys now
		scanKeys();
		// Exit if Start was pressed
		if (keysDown()&KEY_START)
			break;
		if (keysDown()&KEY_A)
			checkTile(T_A);
		if (keysDown()&KEY_Y) {
			menuMain();
		}

		movePlayer();		// handle key input to move player around
		drawMap();
		animatePC(&player);
		moveNPCs();
		animateNPCs();

		oamMain.oamMemory[0].attribute[0] = ATTR0_NORMAL | ATTR0_TYPE_NORMAL | ATTR0_COLOR_256 | ATTR0_TALL
											| OBJ_Y(player.y-Level.y);
		oamMain.oamMemory[0].attribute[1] = ATTR1_SIZE_32 | OBJ_X(player.x-Level.x);
//		oamSet(&oamMain, 0, player.x-Level.x, player.y-Level.y, 1, 0, SpriteSize_32x32, SpriteColorFormat_256Color,
//			   player.sprite_gfx_mem, -1, false, false, false, false, false);
		oamUpdate(&oamMain);
		swiWaitForVBlank();
	}

	return 0;
}
#undef DEBUG
#undef FRAMES_PER_ANIMATION
