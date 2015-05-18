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

void tilemap(u16 map[], int size, int x, int y) {
//	REG_BG0CNT = BG_32x32 | BG_COLOR_16 | BG_MAP_BASE(0) | BG_TILE_BASE(1);
	// Our map will get stored at map base 0
	u16 *tilemap = BG_MAP_RAM(0);
	int i;

	for(i = 0; i < size; i++) {
		*tilemap = map[i];
		tilemap += 1;
	}


//	dmaCopy(map, bgGetMapPtr(bgid), size);
/* I finally got things figured out, arrays get passed as a pointer to
 * the first element in the array. sizeof() returns the size of the
 * pointer, not of the entire array.
	u16 *tilemap = 0x06000000;	int i;
	for(i = 0; i < size; i++) {
		*tilemap = map[i];
		tilemap += 1;
	}*/
}
