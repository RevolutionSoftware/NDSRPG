#ifndef TILEMAP_H
#define TILEMAP_H

typedef struct {
	bool isPassable;	// 0 = false, 1 = true
	u16 bgId;			// which background buffer to draw tile to
}tile_t;

typedef struct {
	u16 *map;
	tile_t *tiles;
	u16 *objs;
	int w,h,x,y;
}map_t;

void tilemap(map_t *Level);

void checkTile(map_t *Level, int playerx, int playery);

#ifdef DEBUG
char* toString(int i);
#endif

#endif
