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

void tilemap(u16 map[], int size, int bgid, int x, int y) {
	dmaCopy(map, bgGetMapPtr(bgid), size);
/* I finally got things figured out, arrays get passed as a pointer to
 * the first element in the array. sizeof() returns the size of the
 * pointer, not of the entire array.
	u16 *tilemap = 0x06000000;	int i;
	for(i = 0; i < size; i++) {
		*tilemap = map[i];
		tilemap += 1;
	}*/
}
