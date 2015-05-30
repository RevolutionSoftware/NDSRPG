#include <nds.h>	// Main NDS equates
#include <stdio.h>
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

void checkTile(map_t *Level, int playerx, int playery) {
	int x = playerx/16;
	int y = playery/16;
	
	int action = -1;
	int i = 0;
	while(Level->objs[i] != -1) {
		if(Level->objs[i] == x && Level->objs[i+1] == y)
			action = i+2;		// skip the x and y coordinates
		i+=4;
	}
	if(action != -1)
		drawTextBox(0,0,32,2,"ACTION! YES! YES! YES!",D_NONE);
		if(Level->objs[action] == 0) {
			putString(0,1,32,"MAP",D_NONE);
//			*Level = map_list[Level->objs[action+1]];
		}
	else {
		char string[100];
		sprintf(string,"Checking... but nada %d",EOF);
		drawTextBox(0,0,32,2,string,D_NONE);
	}
}
