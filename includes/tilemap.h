#ifndef TILEMAP_H
#define TILEMAP_H
#include "objects.h"

/* tile_t:
 * Each tile has data about itself. For now we need to know if the player can walk
 *	on that tile, which background (fg or bg) it should be drawn to, as well as some
 *	flag data.
 * */
typedef struct {
	bool isPassable;	// 0 = false, 1 = true
	u16 bgId;			// which background buffer to draw tile to
	u8 flag;
}tile_t;

/* map_t:
 * Our map struct.
 * Contains a pointer to map data including tilemap, tile brushes, and map objects.
 * *map, *tiles, and *objs are pointers to arrays of data
 * */
typedef struct {
	u16 *map;			// pointer to tilemap
	tile_t *tiles;		// pointer to extra tile info
	s16 *objs;			// pointer to list of map objects
	int w,h,x,y,numNPCs;	// map width and height, starting coords, etc.
}map_t;

enum TileType {
	T_MOTION = 0,
	T_A = 1
};

void drawMap();

void checkTile(int type);

#ifdef DEBUG
char* toString(int i);
#endif

#endif
