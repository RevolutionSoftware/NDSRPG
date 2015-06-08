#include <nds.h>

#include "objects.h"
#include "menus.h"
#include "text.h"
#include "utilities.h"

#include "cursor.h"

// drawMenu: Takes parameters and draws the menu with text inside

// Return: -1 if B was pressed, otherwise return id of option selected.
int drawMenu(int x, int y, int w, char *text) {
	int h = stringHeight(text);

	// load cursor
	SpriteEntry *cursor;
	cursor = initSprite(0, (u8 *)cursorTiles, cursorTilesLen);

	// find coordinates of the menu options in string
	MenuChoice choices[20];
	int tx,ty,i,numChoices;
	numChoices = 0;
	tx = x;			// tx = current x value on screen
	ty = y;			// ty = y value on screen
	i = 0;			// loop through string and store screen coordinates of each \1
	while(text[i] != '\0') {
		switch(text[i]) {
			case '\n':						// at \n reset x and move to next y value
				tx = x-1;
				ty++;
				break;
			case '\1':
				choices[numChoices].x = tx;	// load x and y values of menu option
				choices[numChoices].y = ty;
				numChoices++;
				break;
			default:
				break;
		}
		tx++;
		i++;
	}

    drawTextBox(x, y, w, h, text, D_NONE);

	// turn on cursor
	cursor->isHidden = false;

	int running = true;
	int selOption = 0;
	int animation = 0;
	keysSetRepeat(8,3);
	// Handle key presses
	while(running) {
		animation++;
		cursor->x = choices[selOption].x*8+3+((animation & 0b10000)>>4);
		cursor->y = choices[selOption].y*8+4;
		oamUpdate(&oamSub);

		delay(1);

		scanKeys();
		int keys = keysDownRepeat();

		if(keys & KEY_DOWN && selOption < numChoices - 1) {
			selOption++;
		}
		else if (keysDown()&KEY_DOWN && selOption == numChoices-1) {
			selOption = 0;
		}
		if(keys & KEY_UP && selOption > 0) {
			selOption--;
		}
		else if (keysDown()&KEY_UP && selOption == 0) {
			selOption = numChoices-1;
		}

		if(keys & KEY_A) {
			running = false;
		}
		if(keys & KEY_B) {
			running = false;
		}
		delay(1);
	}
	// turn off cursor
	cursor->isHidden = true;
	oamUpdate(&oamSub);

	delTextBox(x, y, w, h+2);

	if(keysHeld() & KEY_B)
		selOption = -1;
	return selOption;

}

/*
  Within the menus there will be options you can select with a cursor
  controlled by the directional-pad.

  Pressing Y will open the menu, pressing B in the menu will get you
  back to the game.

  NOTE: Should being in the menu pause the game?
  ANSWER: Yes, it should.
*/
