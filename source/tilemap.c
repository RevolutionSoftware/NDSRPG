#include <nds.h>	// Main NDS equates

#include "constants.h"
#include "tilemap.h"
#include "text.h"
#include "utilities.h"
#include "movement.h"
#include "npcs.h"
#include "menus.h"

// Variables declared elsewhere
extern map_t map_list[];				// contains pointers to map information [maps/maps.h]
extern u16 map_change_list[][5];		// data to switch between maps (map id and coordinates) [maps/maps.h]
extern char* text_list[];				// pointers to texts [texts/texts.h]
extern Drawable player;					// main character [main.c]
extern map_t Level;						// map and tile data [main.c]
extern NPC npcs[];						// list of npcs on the current map [main.c]

/* drawTile:
 * Takes a pointer to tilemap (in VRAM) and the id of the tile to draw
 * Tiles are 16x16, so we draw 4 8x8 tiles, so tiles are arranged 4 tiles
 *	in a row.
 */
void drawTile(u16 *tilemap, int tileId) {
	*tilemap = tileId*4;	// store the tile id into background
	tilemap++;				// update pointer
	*tilemap = tileId*4+1;
	tilemap += 31;
	*tilemap = tileId*4+2;
	tilemap++;
	*tilemap = tileId*4+3;
}


/* drawMap:
 * This draws a 17x13 tilemap, with 1 extra tile on the right and bottom
 * to make scrolling easier.
 */
void drawMap() {
    // Our map will get stored at map base 0
    int w = Level.w;
    int x = Level.x >> 4;				// divide by 16
    int y = Level.y >> 4;
    u16 *tilemap_bg = BG_MAP_RAM(0);	// tilemap pointers. background is in bank A
    u16 *tilemap_fg = BG_MAP_RAM(2);	// use map base 0 and 2, each map base is 2kb
    int i;
    int row;
    int start = x+y*w;
    for(row = 0; row < 13; row++) {
        for(i = start; i < start+16; i++) {
			int tileId = Level.map[i]+1;				// the first tile is just a transparent tile
			int tileBG,tileFG;
			if(Level.tiles[(tileId-1)*3].bgId == 0) {	// check if tile should be drawn to fg or bg
				tileBG = tileId;
				tileFG = 0;					// tile 0 is transparent
			} else {
				tileBG = 1;					// tile 1 is ground tile
				tileFG = tileId;
			}
			drawTile(tilemap_bg,tileBG);	// draw the tiles
			drawTile(tilemap_fg,tileFG);
			tilemap_bg+=2;					// each tile is 16x16, so skip two bytes
			tilemap_fg+=2;
        }
		tilemap_bg += 32;					// jump to next row in tilemap base
		tilemap_fg += 32;					// jump to next row in tilemap base
		start += w;							// jump to next row in the tilemap
    }

    // Jump to map_base 1/3 to draw last column of sprites (just off screen)
    tilemap_bg = BG_MAP_RAM(1);
    tilemap_fg = BG_MAP_RAM(3);

    start = x+y*w+16;
    for(row = 0; row < 16; row++) {
		int tileId = Level.map[start]+1;
		int tileBG,tileFG;
		if(Level.tiles[(tileId-1)*3].bgId == 0) {
			tileBG = tileId;
			tileFG = 0;
		} else {
			tileBG = 1;
			tileFG = tileId;
		}
		drawTile(tilemap_bg,tileBG);
		drawTile(tilemap_fg,tileFG);
        tilemap_bg += 64;
        tilemap_fg += 64;
        start += w;
    }
}

/* checks if a tile has an action that should activate when tile is touched
 * 0 = map change
 * 1 = text (activated with B)
 * */
void checkTile(int type) {
	// check if we should talk to an NPC
	if(type == T_A) {
		int i;
		int npc_id=-1;
		for(i = 0; i < Level.numNPCs; i++) {
			if(NPCCollision(i,player.state,-1))
				npc_id = i;
		}
		// if talking to an NPC
		if(npc_id >= 0) {
			// make sure NPC is facing player
			npcs[npc_id].direction = (player.state+2)%4;	// basically add 2 to player's state (direction) and wrap around if necessary
			npcs[npc_id].anim_frame = 0;					// stand still when talking to player
			animateNPCs();

			// display the text!
			drawTextBox(0,0,32,3,0,D_SLOW,text_list[npcs[npc_id].string_id]);
			waitAB();	// wait for player to press [A] or [B]
			// remove text box from screen
			delTextBox(0,0,32,4);
			return;
		}
	}

	int x = player.x;
	int y = player.y+16;	// +15 lets the player overlap the tile a bit
	int x2 = x;
	int y2 = y;

	if(type == T_MOTION) {
		if(player.state == W_UP) {
			x+=8;
			x2 = x;
			y--;
		}
		if(player.state == W_DOWN) {
			x+=8;
			x2 = x;
			y+=10;				// look below player's feet
		}

		if(player.state == W_LEFT) {
			x--;
			y2 += 8;
		}
		if(player.state == W_RIGHT) {
			x+=16;
			y2 += 8;
		}
	} else if(type == T_A) {
		if(player.state == W_UP) {
			x2+=15;
			y--;
		}
		if(player.state == W_DOWN) {
			x2+=15;
			y+=10;				// look below player's feet
		}

		if(player.state == W_LEFT) {
			x--;
			y2 += 8;
		}
		if(player.state == W_RIGHT) {
			x+=16;
			y2 += 8;
		}
	}
	// convert x and y into tile positions
	x/=16;
	y/=16;
	x2/=16;
	y2/=16;

	int action = -1;
	int i = 0;
	int flag = 0;
	int tile;

	while(Level.objs[i] != '\0') {
		if((Level.objs[i] == x || Level.objs[i] == x2) && (Level.objs[i+1] == y || Level.objs[i+1] == y2)) {
			action = i+2;		// skip the x and y coordinates
			x = Level.objs[i];
			y = Level.objs[i+1];
			tile = Level.map[y*Level.w+x];
			flag = Level.tiles[tile*3].flag;
		}
		i+=4;
	}

	// check if there is an action attached to the tile
	if(action != -1) {
		// Change maps
		if(Level.objs[action] == 0 && (flag & 1<<player.state)) {
			// hide player and NPCs
			for(i = 0; i < Level.numNPCs+1; i++)
				oamMain.oamMemory[i].isHidden = true;

			int map_action = Level.objs[action+1];
			int map_id = map_change_list[map_action][0];
			// load the new map into Level
			Level = map_list[map_id];
			loadNPCs(map_id);
			// update the player/map coordinates
			Level.x = map_change_list[map_action][1]*16;
			Level.y = map_change_list[map_action][2]*16;
			player.x = map_change_list[map_action][3]*16;
			player.y = map_change_list[map_action][4]*16;

			// turn on mosaic effect for bg and disable player
			REG_BG0CNT = REG_BG0CNT_DEFAULT | BG_MOSAIC_ON;
			REG_BG1CNT = REG_BG1CNT_DEFAULT | BG_MOSAIC_ON;
			oamUpdate(&oamMain);

			// update mosaic register, first 4 bits are horizontal stretch, 2nd 4 vertical stretch
			int i;
			for(i = 0; i<16; i++) {
				REG_MOSAIC = i+(i<<4);	//+(i<<8)+(i<<12);
				delay(3);
			}

			REG_BG0HOFS = (Level.x)%16;
			REG_BG0VOFS = (Level.y)%16;
			REG_BG1HOFS = (Level.x)%16;
			REG_BG1VOFS = (Level.y)%16;

			drawMap();

			for(i = 15; i>0; i--) {
				REG_MOSAIC = i+(i<<4)+(i<<8)+(i<<12);
				delay(3);
			}

			REG_MOSAIC = 0;
			REG_BG0CNT = REG_BG0CNT_DEFAULT;
			REG_BG1CNT = REG_BG1CNT_DEFAULT;

		}
		// texts with [A]
		if(Level.objs[action] == 1 && type == T_A) {
			int text_id = Level.objs[action+1];
			// display the text!
			drawTextBox(0,0,32,2,0,D_SLOW,text_list[text_id]);
			waitAB();				// wait for player to press [A] or [B]
			delTextBox(0,0,32,3);
		}
		// load a store with [A]
		if(Level.objs[action] == 2 && type == T_A) {
			int store_id = Level.objs[action+1];
			menuStore(store_id);
		}
	}
}
