#include <nds.h>

#include "menus.h"
#include "text.h"


// drawMenu: Takes parameters and draws the menu with text inside

// Return: Nothing.
void drawMenu(int x, int y, int w, int h, char *text) {
    drawBox(x, y, w, h+1);
    putString(x, y, w-1, text, D_NONE);
}

// initCursor: Initiallizes the cursor sprite and such into memory.
// Notes: Strangely similar to init_PC. ;-)
// Returns: Nothing.
void initCursor(Drawable *cursor, u8 *gfx) {
    cursor->sprite_gfx_mem = oamAllocateGfx(&oamSub, SpriteSize_8x8, SpriteColorFormat_16Color);
    cursor->frame_gfx = (u8 *)gfx;
}

// drawCursor:
//
void drawCursor(Drawable *cursor) {
    // It's also being animated whilst being drawn..
    int cursor_blink = cursor->anim_frame/8;

    if (cursor_blink > 1)
        cursor_blink = (cursor_blink%2)*2;

    int frame = cursor_blink + cursor->state * 3;
    int *offset = cursor->frame_gfx + frame * 8 * 8;

    dmaCopy(offset, cursor->sprite_gfx_mem, 8 * 8);

}

// selectOption:
//
// Return:
int selectOption () {
    int option = 0;

    return option;
}

/*
  Within the menus there will be options you can select with a cursor
  controlled by the directional-pad.

  Pressing Y will open the menu, pressing B in the menu will get you
  back to the game.

  NOTE: Should being in the menu pause the game?
  ANSWER: Yes, it should.
*/
