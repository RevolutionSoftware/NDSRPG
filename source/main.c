/* 
 * main.c
 * Authors: Chickendude, add
 */

// Includes NDS
#include <nds.h>	// Main NDS equates
#include <stdio.h>	// For console stuff
#include <stdint.h>
#include "font.h"
#include "tiles.h"

#include "aux_macros.h"

/* Each map entry is apparently 2 bytes, since a tileset can hold up
 * to 1024 tiles. Map is currently exactly same size as screen,
 * 32 8x8 tiles wide, 24 tiles tall.
 */
global_variable uint16_t map[] = {
    0,1,4,5,0,1,0,1,0,1,0,1,0,1,0,1,0,1,4,5,0,1,0,1,0,1,0,1,0,1,0,1,
	2,3,6,7,2,3,2,3,2,3,2,3,2,3,2,3,2,3,6,7,2,3,2,3,2,3,2,3,2,3,2,3,
	0,1,4,5,0,1,0,1,0,1,0,1,0,1,0,1,0,1,4,5,0,1,0,1,0,1,0,1,0,1,0,1,
	2,3,6,7,2,3,2,3,2,3,2,3,2,3,2,3,2,3,6,7,2,3,2,3,2,3,2,3,2,3,2,3,
	0,1,4,5,0,1,0,1,0,1,0,1,0,1,0,1,0,1,4,5,0,1,0,1,0,1,0,1,0,1,0,1,
	2,3,6,7,2,3,2,3,2,3,2,3,2,3,2,3,2,3,6,7,2,3,2,3,2,3,2,3,2,3,2,3,
	0,1,4,8,8,8,0,8,8,1,0,8,8,1,0,1,0,1,4,5,0,1,0,1,0,1,0,1,0,1,0,1,
	2,3,6,8,2,8,2,8,2,8,2,8,2,8,2,3,2,3,6,7,2,3,2,3,2,3,2,3,2,3,2,3,
	0,1,4,8,8,8,0,8,0,8,0,8,0,8,0,1,0,1,4,5,0,1,12,13,0,1,0,1,0,1,0,1,
	2,3,6,8,2,8,2,8,2,8,2,8,2,8,2,3,2,3,6,7,2,3,14,15,2,3,2,3,2,3,2,3,
	0,1,4,8,0,8,0,8,8,1,0,8,8,1,0,1,0,1,4,5,0,1,0,1,0,1,0,1,0,1,0,1,
	2,3,6,7,2,3,2,3,2,3,2,3,2,3,2,3,2,3,6,7,2,3,2,3,2,3,2,3,2,3,2,3,
	0,1,4,5,0,1,0,1,0,1,0,1,0,1,0,1,0,1,4,5,0,1,0,1,0,1,0,1,0,1,0,1,
	2,3,6,7,2,3,2,3,2,3,2,3,2,3,2,3,2,3,6,7,2,3,2,3,2,3,2,3,2,3,2,3,
	0,1,4,5,0,1,0,1,0,1,0,1,8,9,0,1,0,1,4,5,0,1,0,1,0,1,0,1,0,1,0,1,
	2,3,6,7,2,3,2,3,2,3,2,3,10,11,2,3,2,3,6,7,2,3,2,3,2,3,2,3,2,3,2,3,
	0,1,4,5,0,1,0,1,0,1,0,1,0,1,0,1,0,1,4,5,0,1,0,1,0,1,0,1,0,1,0,1,
	2,3,6,7,2,3,2,3,2,3,2,3,2,3,2,3,2,3,6,7,2,3,2,3,2,3,2,3,2,3,2,3,
	0,1,4,5,0,1,0,1,0,1,0,1,0,1,0,1,0,1,4,5,0,1,0,1,0,1,0,1,0,1,0,1,
	2,3,6,7,2,3,2,3,2,3,2,3,2,3,2,3,2,3,6,7,2,3,2,3,2,3,2,3,2,3,2,3,
	0,1,4,5,0,1,0,1,0,1,0,1,0,1,0,1,0,1,4,5,0,1,0,1,0,1,0,1,0,1,0,1,
	2,3,6,7,2,3,2,3,2,3,2,3,2,3,2,3,2,3,6,7,2,3,2,3,2,3,2,3,2,3,2,3,
	0,1,4,5,0,1,0,1,0,1,0,1,0,1,0,1,0,1,4,5,0,1,0,1,0,1,0,1,0,1,0,1,
	2,3,6,7,2,3,2,3,2,3,2,3,2,3,2,3,2,3,6,7,2,3,2,3,2,3,2,3,2,3,2,3,
};


int main(void) {
    int bg0;
	consoleDemoInit();
	iprintf("NDSRPG:\n");
	iprintf("4-bit color tilemap");

/* NDS has nine memory banks, banks 0-4
 *  Use mode 0. Mode 0 is for tilebased sprites, called "text" mode
 * Other modes have options for rotation, scaling, and bitmap display
 * You have access to 4 backgrounds in mode 0
 */
	videoSetMode(MODE_0_2D | DISPLAY_BG0_ACTIVE);   // Set mode 0 in 2D mode (not 3D)
    vramSetBankA(VRAM_A_MAIN_BG);					// There are nine memory banks, use memory bank A
    /* Initializes the background:
     * 	"Text" essentially means the same thing as "Tile"
     * Text4bpp means our tiles are 4bpp
     * static int bgInit (int layer (0-3), BgType type, BgSize size,
     *                    int mapBase (2kb offset in VRAM), int tileBase (16kb offset in VRAM))
     * There are other sizes available other than 256x256, eg. 512x256 and 512x512
     * I think that's what we'll use for scrolling our maps
     */
	bg0 = bgInit(0, BgType_Text4bpp, BgSize_T_256x256, 0,1);

/* use DMA to copy data over
 * bgGetGfxPtr gets the uses the id from bgInit (or bgInitSub, which works with the bottom screen)
 */
	dmaCopy(tilesTiles, bgGetGfxPtr(bg0), tilesTilesLen);
	dmaCopy(map, bgGetMapPtr(bg0), sizeof(map));
	dmaCopy(tilesPal, BG_PALETTE, 256*2);
	while(1) {
        swiWaitForVBlank();
		scanKeys();
		// Exit if Start was pressed
		if (keysDown()&KEY_START)
            break;
	}

	return 0;
}
