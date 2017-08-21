#ifndef CONSTANTS_H
#define CONSTANTS_H
#define REG_BG0CNT_DEFAULT BG_PRIORITY_0 | BG_64x32 | BG_COLOR_256 | BG_MAP_BASE(2) | BG_TILE_BASE(1)	// map base = 2kb, tile base = 16kb
#define REG_BG1CNT_DEFAULT BG_PRIORITY_3 | BG_64x32 | BG_COLOR_256 | BG_MAP_BASE(0) | BG_TILE_BASE(1)	// map base = 2kb, tile base = 16kb

#define PLAYER_WIDTH	16
#define PLAYER_HEIGHT	24
#define PLAYER_Y_OVERLAP	16	// how many pixels player overlaps tile
#define SPEED			2
#define ANIMATION_SPEED	8

enum {
	SCREEN_TOP = 0,
	SCREEN_BOTTOM = 192,
	SCREEN_LEFT = 0,
	SCREEN_RIGHT = 256
};

enum SpriteState {
    W_UP = 0,
    W_RIGHT = 1,
    W_DOWN = 2,
    W_LEFT = 3
};
#endif
