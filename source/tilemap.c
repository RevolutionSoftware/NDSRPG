#include <nds.h>	// Main NDS equates
#include <stdio.h>
#include "tilemap.h"

#ifdef DEBUG
char* toString(int i) {
	char string[5];
	sprintf(string,"%d",i);
	return string;
}
#endif

/* This draws a 17x13 tilemap, with 1 extra tile on the right and bottom
 * to make scrolling easier.
 */
void tilemap(u16 map[], int w, int h, int x, int y) {
	// Our map will get stored at map base 0
	u16 *tilemap = BG_MAP_RAM(0);
	int i;

// debug stuff
	char str[100];
	sprintf(str,"\nx: %d, y: %d, w: %d",x,y,w);
	if (keysDown()&KEY_A)
		iprintf(str);
// #################

	int row;
	int start = x+y*w;
	for(row = 0; row < 13; row++) {
		for(i = start; i < start+16; i++) {
			*tilemap = map[i]*4;
			tilemap++;
			*tilemap = map[i]*4+1;
			tilemap += 31;
			*tilemap = map[i]*4+2;
			tilemap++;
			*tilemap = map[i]*4+3;
			tilemap -= 31;
		}
		tilemap += 32;	// jump to next row in tilemap base
		start += w;		// jump to next row in the tilemap
	}

	// Jump to map_base 1 to draw last column of sprites (just off screen)
	tilemap = BG_MAP_RAM(1);

	start = x+y*w+16;
	for(row = 0; row < 16; row++) {
		*tilemap = map[start]*4;
		tilemap++;
		*tilemap = map[start]*4+1;
		tilemap += 31;
		*tilemap = map[start]*4+2;
		tilemap++;
		*tilemap = map[start]*4+3;
		tilemap += 31;
		start += w;
	}
}
