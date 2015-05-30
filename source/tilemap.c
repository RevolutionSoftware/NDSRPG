#include <nds.h>	// Main NDS equates
#include <stdio.h>	// For console stuff
#include "tilemap.h"
#include "text.h"

/* This draws a 17x13 tilemap, with 1 extra tile on the right and bottom
 * to make scrolling easier.
 */
void drawMap(map_t *Level) {
    // Our map will get stored at map base 0
    int w = Level->w;
    int x = Level->x >> 4;
    int y = Level->y >> 4;
    u16 *tilemap = BG_MAP_RAM(0);
    int i;
    int row;
    int start = x+y*w;
    for(row = 0; row < 13; row++) {
        for(i = start; i < start+16; i++) {
            *tilemap = Level->map[i]*4;
            tilemap++;
            *tilemap = Level->map[i]*4+1;
            tilemap += 31;
            *tilemap = Level->map[i]*4+2;
            tilemap++;
            *tilemap = Level->map[i]*4+3;
            tilemap -= 31;
        }
        tilemap += 32;	// jump to next row in tilemap base
        start += w;		// jump to next row in the tilemap
    }

    // Jump to map_base 1 to draw last column of sprites (just off screen)
    tilemap = BG_MAP_RAM(1);

    start = x+y*w+16;
    for(row = 0; row < 16; row++) {
        *tilemap = Level->map[start]*4;
        tilemap++;
        *tilemap = Level->map[start]*4+1;
        tilemap += 31;
        *tilemap = Level->map[start]*4+2;
        tilemap++;
        *tilemap = Level->map[start]*4+3;
        tilemap += 31;
        start += w;
    }
}

extern map_t map_list[];
extern u16 map_change_list[][5];

void checkTile(map_t *Level, PC_t *player, int playerx, int playery) {
	int x = playerx/16;
	int y = (playery+15)/16;

	int action = -1;
	int i = 0;

	while(Level->objs[i] != EOF) {
		if(Level->objs[i] == x && Level->objs[i+1] == y)
			action = i+2;		// skip the x and y coordinates
		i+=4;
	}

	char debugStr[100];
	sprintf(debugStr,"%d\nAction: %d",Level->objs[i],action);
	drawTextBox(0,7,32,3,debugStr, D_NONE);


	if(action != -1) {
		drawTextBox(0,0,32,2,"ACTION! YES! YES! YES!",D_NONE);
		if(Level->objs[action] == 0) {
			putString(0,1,32,"MAP",D_NONE);
			int map_action = Level->objs[action+1];
			int map_id = map_change_list[map_action][0];
			*Level = map_list[map_id];
			Level->x = map_change_list[map_action][1]*16;
			Level->y = map_change_list[map_action][2]*16;
			player->x = map_change_list[map_action][3]*16;
			player->y = map_change_list[map_action][4]*16;
			delay(15);
		}
	}
	else {
		char string[100];
		drawTextBox(0,0,32,2,"Checking... but nada",D_NONE);
	}
}
