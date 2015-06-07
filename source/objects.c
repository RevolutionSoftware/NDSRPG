#include <nds.h>

#include "objects.h"

/* initSpriteSub:
 * 	sets up a sprite in sub memory
 * Returns: Nothing.
 * */
SpriteEntry* initSprite(int spriteid, u8 *gfx, int len) {
	SpriteEntry *sprite = &oamSub.oamMemory[spriteid];

	dmaCopy(gfx, SPRITE_GFX_SUB+spriteid*64, len);	// copy sprite to sprite area, assuming 8x8

	sprite->attribute[0] = ATTR0_NORMAL | ATTR0_TYPE_NORMAL | ATTR0_COLOR_256 | ATTR0_SQUARE | OBJ_Y(8);
	sprite->attribute[1] = ATTR1_SIZE_8 | OBJ_X(8);
	sprite->attribute[2] = spriteid;	// priority 0
	
	sprite->x = 100;
	return sprite;
}
