#ifndef TILEMAP_H
#define TILEMAP_H

typedef struct {
	bool isPassable;	// 0 = false, 1 = true
	u16 textId;			// -1 = no text, 0+ = string to display when activated with [A]
	u8 mapId;			// -1 = no jump, otherwise map to switch to
	u16 mapX;
	u16 mapY;
	u16 playerX;		// player's coordinates in the new map
	u16 playerY;
}tile_t;

void tilemap(u16 map[], int w, int h, int x, int y);

#ifdef DEBUG
char* toString(int i);
#endif

#endif
