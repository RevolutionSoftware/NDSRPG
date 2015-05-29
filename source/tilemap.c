#include <nds.h>	// Main NDS equates
#include <stdio.h>
#include "tilemap.h"


/* This draws a 17x13 tilemap, with 1 extra tile on the right and bottom
 * to make scrolling easier.
 */
void tilemap(map_t *Level) {
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

int checkTile(map_t *Level, int x, int y) {
	
	return 0;
}
