
//{{BLOCK(player)

//======================================================================
//
//	player, 96x128@8, 
//	+ palette 256 entries, not compressed
//	+ 192 tiles Metatiled by 4x4 not compressed
//	Total size: 512 + 12288 = 12800
//
//	Time-stamp: 2015-05-18, 23:27:37
//	Exported by Cearn's GBA Image Transmogrifier, v0.8.12
//	( http://www.coranac.com/projects/#grit )
//
//======================================================================

#ifndef GRIT_PLAYER_H
#define GRIT_PLAYER_H

#define PLAYER_WIDTH 17
#define PLAYER_HEIGHT 24
#define SPEED 2
#define ANIMATION_SPEED 8

#define playerTilesLen 12288
extern const unsigned int playerTiles[3072];

#define playerPalLen 512
extern const unsigned short playerPal[256];

#endif // GRIT_PLAYER_H

//}}BLOCK(player)
