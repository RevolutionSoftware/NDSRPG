#ifndef TILEMAP_H
#define TILEMAP_H
#include "objects.h"

typedef struct {
	bool isPassable;	// 0 = false, 1 = true
	u16 bgId;			// which background buffer to draw tile to
	u8 flag;
}tile_t;

typedef struct {
	u16 *map;
	tile_t *tiles;
	s16 *objs;
	int w,h,x,y;
}map_t;

enum TileType {
	T_MOTION = 0,
	T_A = 1
};

void drawMap(map_t *Level);

void checkTile(map_t *Level, Drawable *player, int type);

#ifdef DEBUG
char* toString(int i);
#endif

#endif
