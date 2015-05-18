#ifndef OBJECTS_H
#define OBJECTS_H

#define PCTilesLen 12288
extern const unsigned int playerTiles[3072];

#define PCPalLen 512
extern const unsigned short playerPal[256];

// We can make the Player-Character datatype better soon enough.
typedef struct {
	int x;
	int y;

	u16 *sprite_gfx_mem;
	u8  *frame_gfx;

	int state;
	int anim_frame;
}PC;

#endif /* OBJECTS_H */
