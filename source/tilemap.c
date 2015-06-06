#include <nds.h>	// Main NDS equates
#include <stdio.h>	// For console stuff
#include "tilemap.h"
#include "text.h"
#include "utilities.h"
#include "movement.h"

void drawTile(u16 *tilemap, int tileId) {
	*tilemap = tileId*4;
	tilemap++;
	*tilemap = tileId*4+1;
	tilemap += 31;
	*tilemap = tileId*4+2;
	tilemap++;
	*tilemap = tileId*4+3;
}


/* This draws a 17x13 tilemap, with 1 extra tile on the right and bottom
 * to make scrolling easier.
 */
void drawMap(map_t *Level) {
    // Our map will get stored at map base 0
    int w = Level->w;
    int x = Level->x >> 4;
    int y = Level->y >> 4;
    u16 *tilemap_bg = BG_MAP_RAM(0);
    u16 *tilemap_fg = BG_MAP_RAM(2);
    int i;
    int row;
    int start = x+y*w;
    for(row = 0; row < 13; row++) {
        for(i = start; i < start+16; i++) {
			int tileId = Level->map[i]+1;			// the first tile is just a transparent tile
			int tileBG,tileFG;
			if(Level->tiles[(tileId-1)*3].bgId == 0) {
				tileBG = tileId;
				tileFG = 0;
			} else {
				tileBG = 1;
				tileFG = tileId;
			}
			drawTile(tilemap_bg,tileBG);
			drawTile(tilemap_fg,tileFG);
			tilemap_bg+=2;
			tilemap_fg+=2;
        }
		tilemap_bg += 32;	// jump to next row in tilemap base
		tilemap_fg += 32;	// jump to next row in tilemap base
		start += w;		// jump to next row in the tilemap
    }

    // Jump to map_base 1/3 to draw last column of sprites (just off screen)
    tilemap_bg = BG_MAP_RAM(1);
    tilemap_fg = BG_MAP_RAM(3);

    start = x+y*w+16;
    for(row = 0; row < 16; row++) {
		int tileId = Level->map[start]+1;
		int tileBG,tileFG;
		if(Level->tiles[(tileId-1)*3].bgId == 0) {
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

extern map_t map_list[];
extern u16 map_change_list[][5];
extern char* text_list[];

/* checks if a tile has an action that should activate when tile is touched
 * 0 = map change
 * 1 = text (activated with B)
 * */
void checkTile(map_t *Level, Drawable *player, int type) {
	int x = player->x;
	int y = player->y+16;	// +15 lets the player overlap the tile a bit
	int x2 = x;
	int y2 = y;

	if(type == T_MOTION) {
		if(player->state == W_UP) {
			x+=8;
			x2 = x;
			y--;
		}
		if(player->state == W_DOWN) {
			x+=8;
			x2 = x;
			y+=10;				// look below player's feet
		}

		if(player->state == W_LEFT) {
			x--;
			y2 += 8;
		}
		if(player->state == W_RIGHT) {
			x+=16;
			y2 += 8;
		}
	} else if(type == T_A) {
		if(player->state == W_UP) {
			x2+=15;
			y--;
		}
		if(player->state == W_DOWN) {
			x2+=15;
			y+=10;				// look below player's feet
		}

		if(player->state == W_LEFT) {
			x--;
			y2 += 8;
		}
		if(player->state == W_RIGHT) {
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
	int flag=0;
	int tile;

	while(Level->objs[i] != EOF) {
		if((Level->objs[i] == x || Level->objs[i] == x2) && (Level->objs[i+1] == y || Level->objs[i+1] == y2)) {
			action = i+2;		// skip the x and y coordinates
			x = Level->objs[i];
			y = Level->objs[i+1];
			tile = Level->map[y*Level->w+x];
			flag = Level->tiles[tile*3].flag;
		}
		i+=4;
	}

	// check if there is an action attached to the tile
	if(action != -1) {
		int keys = keysHeld();
		// maps
		if(Level->objs[action] == 0 && (flag & 1<<player->state)) {
			int map_action = Level->objs[action+1];
			int map_id = map_change_list[map_action][0];
			*Level = map_list[map_id];
			Level->x = map_change_list[map_action][1]*16;
			Level->y = map_change_list[map_action][2]*16;
			player->x = map_change_list[map_action][3]*16;
			player->y = map_change_list[map_action][4]*16;
			delay(15);
			REG_BG1HOFS = (Level->x)%16;
			REG_BG1VOFS = (Level->y)%16;
		}
		// texts with [A]
		if(Level->objs[action] == 1 && keys&KEY_A) {
			int text_id = Level->objs[action+1];
			// display the text!
			drawTextBox(0,0,32,2,text_list[text_id],D_SLOW);
			waitAB();	// wait for player to press [A] or [B]
			// remove text box from screen
//			delTextBox(0,0,32,3);
		}
	}
}
