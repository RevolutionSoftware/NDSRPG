/* File: main.c
 * Authors: Chickendude, add
 * Description: File containing main */

#define DEBUG



// Includes NDS
#include <nds.h>	// Main NDS equates
#include <stdio.h>	// For console stuff
#include "aux_macros.h"
#include "tilemap.h"
#include "font.h"
#include "tiles.h"
#include "player.h"
#include "objects.h"
#include "movement.h"
#include "text.h"

// map data
#include "maps.h"

// tilemap array, tile data array, interaction array, width, height



#define FRAMES_PER_ANIMATION 3

// Equates
enum {
	SCREEN_TOP = 0,
	SCREEN_BOTTOM = 192,
	SCREEN_LEFT = 0,
	SCREEN_RIGHT = 256
};

enum {
	M_BASE0 = 0,
	M_BASE1 = 1,
	M_BASE2 = 2,
	M_BASE3 = 3,

	T_BASE0 = 0,
	T_BASE1 = 1
};

int main(void) {
	PC_t player = {16,32};
	map_t Level = map_list[1];
	Level.x = 0;
	Level.y = 0;

	/* NDS has nine memory banks, banks 0-4
	 *  Use mode 0. Mode 0 is for tilebased sprites, called "text" mode
	 * Other modes have options for rotation, scaling,
	 * and bitmap display. You have access to 4 backgrounds in mode 0 */
// background
	vramSetBankA(VRAM_A_MAIN_BG_0x06000000);
// main character
	vramSetBankB(VRAM_B_MAIN_SPRITE_0x06400000);
// text
	vramSetBankC(VRAM_C_SUB_BG);
//	vramSetBankH(VRAM_H_SUB_BG);

	REG_DISPCNT = MODE_0_2D | DISPLAY_BG0_ACTIVE;   // Set mode 0 in 2D mode and enable background 0
	REG_BG0CNT = BG_PRIORITY_0 | BG_64x32 | BG_COLOR_256 | BG_MAP_BASE(0) | BG_TILE_BASE(1);	// map base = 2kb, tile base = 16kb

// Font stuff for bottom screen
// Sub screen uses bg1, sub map base 0 and sub tile base 1.
	REG_DISPCNT_SUB = MODE_0_2D | DISPLAY_BG0_ACTIVE| DISPLAY_BG1_ACTIVE;   // bottom screen, use bg0 and bg1
	REG_BG0CNT_SUB = BG_PRIORITY_2 | BG_32x32 | BG_COLOR_16 | BG_MAP_BASE(0) | BG_TILE_BASE(1);
	REG_BG1CNT_SUB = BG_PRIORITY_3 | BG_32x32 | BG_COLOR_16 | BG_MAP_BASE(1) | BG_TILE_BASE(1);

// Copy font data (sprites and palette)
	dmaCopy(fontTiles, BG_TILE_RAM_SUB(1), fontTilesLen);
	dmaCopy(fontPal, BG_PALETTE_SUB, fontPalLen);

//	BG_PALETTE_SUB[0] = RGB15(31, 0, 31);
//	BG_PALETTE_SUB[1] = RGB15(31, 31, 31);

	oamInit(&oamMain, SpriteMapping_1D_128, false);

	init_PC(&player, (u8 *)playerTiles);
	dmaCopy(playerPal, SPRITE_PALETTE, 512);

	/* use DMA to copy data over
	 * bgGetGfxPtr gets the uses the id from bgInit
	 * (or bgInitSub, which works with the bottom screen) */
	// tile_ram is divided into blocks of 16kb, tileram(1) = tile_ram + 16kb
	dmaCopy(tilesTiles, BG_TILE_RAM(1), tilesTilesLen);
	dmaCopy(tilesPal, BG_PALETTE, 256*2);
// **********************************************************************************************

	REG_BG0HOFS = 0;
	REG_BG0VOFS = 0;

	// shift the text a few pixels down and to the right to not collide with the box border
	REG_BG0HOFS_SUB = -4;
	REG_BG0VOFS_SUB = -5;

	// ############# Box drawn around debug values ##################
	drawTextBox(1,3,30,20,"Here we can put some other stats and information, or menus, or options, or bananas, or...\n\nI guess there are still a couple bugs in the text routine. We also need border detection (a width value for how far the text can be drawn without going outside the box).", D_NONE);
	// ##############################################################

	while(1) {
		// ############# DEBUG ################
		char debugStr[100];
		sprintf(debugStr,"X: %d, Y: %d    \nX TILE: %d, Y TILE: %d     ",
				player.x, player.y, (player.x + PLAYER_WIDTH)/16, (player.y + 16)/16);
		drawTextBox(0,0,32,2,debugStr, D_NONE);
		// ####################################

		// Check for keys now
		scanKeys();
		int keys = keysHeld();
		// Exit if Start was pressed
		if (keysDown()&KEY_START)
			break;
		if (keysDown()&KEY_A)
			drawTextBox(0,20,31,1,"You are pressing A",D_NONE);
		if (keysUp()&KEY_A) {
			drawTextBox(0,20,31,1,"You released A",D_NONE);
		}
		if (keys & KEY_RIGHT)
		{
			int mapOffset = (player.y + 16)/16*Level.w+(player.x + PLAYER_WIDTH + SPEED)/16;
			int tileId = Level.map[mapOffset];
			int tileId2 = tileId;
			if (((player.y+16) % 16) >= 8)
				tileId2 = Level.map[mapOffset+Level.w];	// move down one row in map
			// Make sure player isn't at edge of screen
			if (player.x < Level.w*16-PLAYER_WIDTH)
				if(Level.tiles[tileId*2].isPassable && Level.tiles[tileId2*2].isPassable)
					player.x+=SPEED;
			// Make sure map isn't at edge of screen
			if ((player.x-Level.x > SCREEN_RIGHT/2 - PLAYER_WIDTH/2) && Level.x < Level.w*16-16*16)
				Level.x+=SPEED;
			player.state = W_RIGHT;
		}
		if (keys & KEY_LEFT)
		{
			int mapOffset = (player.y+16)/16*Level.w+(player.x-SPEED)/16;
			int tileId = Level.map[mapOffset];
			int tileId2 = tileId;
			if ((player.y + 16) % 16 >= 8)
				tileId2 = Level.map[mapOffset+Level.w];	// move down one row in map
			if (player.x > 0)
				if(Level.tiles[tileId*2].isPassable && Level.tiles[tileId2*2].isPassable)
					player.x-=SPEED;
			if ((player.x-Level.x < SCREEN_RIGHT/2 - PLAYER_WIDTH/2) && Level.x>0)
				Level.x-=SPEED;
			player.state = W_LEFT;
		}
		if (keys & KEY_DOWN)		// w*tile_height - tile_height*tiles_per_column
		{
			int mapOffset = (player.y+PLAYER_HEIGHT+SPEED)/16*Level.w+player.x/16;
			int tileId = Level.map[mapOffset];
			int tileId2 = tileId;
			if (player.x % 16 != 0)
				tileId2 = Level.map[mapOffset+1];
			if (player.y < Level.h*16 - PLAYER_HEIGHT)
				if(Level.tiles[tileId*2].isPassable && Level.tiles[tileId2*2].isPassable)
					player.y+=SPEED;
			if ((player.y-Level.y > SCREEN_BOTTOM/2 - PLAYER_HEIGHT/2) && Level.y < Level.h*16-12*16)
				Level.y+=SPEED;
			player.state = W_DOWN;
		}
		if (keys & KEY_UP)
		{
			int mapOffset = (player.y+16-SPEED)/16*Level.w+player.x/16;
			int tileId = Level.map[mapOffset];
			int tileId2 = tileId;
			if (player.x % 16 != 0)
				tileId2 = Level.map[mapOffset+1];
			if (player.y > 0)
				if(Level.tiles[tileId*2].isPassable && Level.tiles[tileId2*2].isPassable)
					player.y -= SPEED;
			if ((player.y-Level.y < SCREEN_BOTTOM/2 - PLAYER_HEIGHT/2) && Level.y > 0)
				Level.y-=SPEED;
			player.state = W_UP;
			checkTile(&Level,&player,player.x+8,player.y);
		}
		// draw tilemap
		drawMap(&Level);
		if ((keys&KEY_LEFT) | (keys&KEY_RIGHT) | (keys&KEY_DOWN) | (keys&KEY_UP)) {
			REG_BG0HOFS = (Level.x)%16;
			REG_BG0VOFS = (Level.y)%16;
			player.anim_frame++;
			player.anim_frame %= (FRAMES_PER_ANIMATION+1)*ANIMATION_SPEED;
		} else
			player.anim_frame = 0;	// reset animation when not moving
		animate_PC(&player);
		oamSet(&oamMain, 0, player.x-Level.x, player.y-Level.y, 0, 0, SpriteSize_32x32, SpriteColorFormat_256Color,
			   player.sprite_gfx_mem, -1, false, false, false, false, false);
		swiWaitForVBlank();
		oamUpdate(&oamMain);
	}

	return 0;
}
#undef DEBUG
#undef FRAMES_PER_ANIMATION
