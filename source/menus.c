#include <nds.h>

#include "objects.h"
#include "menus.h"
#include "text.h"
#include "utilities.h"
#include "player.h"
#include "weapons.h"
#include "armor.h"
#include "items.h"

// sprites
#include "cursor.h"

// drawMenu: Takes parameters and draws the menu with text inside

// Return: -1 if B was pressed, otherwise return id of option selected.
int drawMenu(int x, int y, int w, char *text, int defValue) {
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
	int selOption = defValue;
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

// ************** specific menus **************************//
extern Party party;
extern Weapon weapon_list[];
extern Armor armor_list[];
extern Item item_list[];
extern Store store_list[];

// menus
extern char *menu_Y;
extern char *menu_items;
extern char *menu_equipment;
extern char *menu_stats;
extern char *menu_options;
extern char *menu_save;

/***************
 * Displays the main menu that pops up when you press Y
 * From here it extends into the other menus
 ***************/
void menuMain() {
	int selected=0;
	while(selected != -1) {
		clrSubScreen();
		selected = drawMenu(0,0,11,menu_Y,selected);		// draw the main menu
		clrSubScreen();
		switch(selected) {							// check which option was chosen
			case 0:
				drawTextBox(0,0,32,23,menu_items,D_NONE);
				waitAB();
				break;
			case 1:
				drawTextBox(0,0,32,23,menu_equipment,D_NONE);
				waitAB();
				break;
			case 2:
				menuStats();
				break;
			case 3:
				drawTextBox(0,0,32,23,menu_options,D_NONE);
				waitAB();
				break;
			case 4:
				drawTextBox(0,0,32,23,menu_save,D_NONE);
				waitAB();
				break;
			default:
				selected = -1;
		}
		releaseKeys();
		clrSubScreen();
	}
}

void menuStats() {
	drawBox(0,0,32,24);
	char *name, *weapon, *armor;
	int hp,hp_max,str,def,agi;
	int i = 0;
	bool running = true;
	while(running) {
		name = party.member[i].name;
		hp = party.member[i].hp;
		hp_max = party.member[i].hp_max;
		str = party.member[i].str;
		def = party.member[i].def;
		agi = party.member[i].agi;
		weapon = weapon_list[party.member[i].wId].name;
		armor = armor_list[party.member[i].aId].name;

		// display the stats screen with the stats inserted into it
		putString(0,0,31,23,D_NONE,menu_stats,name,hp,hp_max,str,def,agi,weapon,armor);

		int keys = waitKey();

		if (keys & KEY_RIGHT && party.member[i+1].active && i < 2) {
			delText(0,0,31,23);
			i++;
		}
		if (keys & KEY_LEFT && i > 0) {
			delText(0,0,31,23);
			i--;
		}
		if (keys & KEY_B)
			running = false;
	}
}

#define ITEMS_ON_SCREEN 19

/*********************
 * handle the shops
 *********************/
void menuStore(int id) {
	// display the text!
	drawTextBox(0,0,32,2,store_list[id].hello,D_SLOW);
	waitAB();				// wait for player to press [A] or [B]
	clrSubScreen();


	drawBox(0,0,32,20);	// main item box
	drawBox(0,20,32,4);	// item description box

	// load cursor
	SpriteEntry *cursor;
	cursor = initSprite(0, (u8 *)cursorTiles, cursorTilesLen);
	cursor->isHidden = false;

	int running = true;
	int selOption = 0;
	int topOption = 0;
	int numItems = store_list[id].numItems;
	int animation = 0;
	keysSetRepeat(16,5);
	// Handle key presses
	while(running) {
		animation++;
		cursor->x = 3 + ((animation & 0b10000)>>4);
		cursor->y = (selOption-topOption)*8+4;
		oamUpdate(&oamSub);

		delay(1);
		drawStore(id,topOption,selOption);

		scanKeys();
		int keys = keysDownRepeat();

		if(keys & KEY_DOWN && selOption < numItems - 1) {
			selOption++;
			if((selOption > topOption + ITEMS_ON_SCREEN-1) && (topOption + ITEMS_ON_SCREEN < numItems))
				topOption++;
		}
		else if (keysDown()&KEY_DOWN && selOption == numItems-1) {
			topOption = 0;
			selOption = 0;
		}
		if(keys & KEY_UP && selOption > 0) {
			selOption--;
			if((selOption < topOption) && (topOption > 0))
				topOption--;
		}
		else if (keysDown()&KEY_UP && selOption == 0) {
			topOption = numItems-ITEMS_ON_SCREEN;
			selOption = numItems-1;
		}

		if(keys & KEY_A) {
			// buy item
			running = false;
		}
		if(keys & KEY_B) {
			// exit store
			running = false;
		}
	}
	// turn off cursor
	cursor->isHidden = true;
	oamUpdate(&oamSub);

	clrSubScreen();

	// display the text!
	drawTextBox(0,0,32,2,store_list[id].goodbye,D_SLOW);
	waitAB();				// wait for player to press [A] or [B]
	clrSubScreen();

	releaseKeys();

}

void drawStore(int storeId, int start, int selected) {
	delText(0,0,32,24);
	int i,numItems;
	numItems = store_list[storeId].numItems;
	if(numItems + start > ITEMS_ON_SCREEN)
		numItems = ITEMS_ON_SCREEN;
	for(i = 0; i < numItems; i++) {
		int itemType = store_list[storeId].items[(i+start)*2];
		int itemId = store_list[storeId].items[(i+start)*2+1];
		int itemPrice = 0;
		char *itemName = "";
		char *itemDesc = "";

		switch(itemType) {
			case 0:		// item
				itemName = item_list[itemId].name;
				itemDesc = item_list[itemId].description;
				itemPrice = item_list[itemId].cost;
				break;
			case 1:		// armor
				itemName = armor_list[itemId].name;
				itemDesc = armor_list[itemId].description;
				itemPrice = armor_list[itemId].cost;
				break;
			case 2:		// weapon
				itemName = weapon_list[itemId].name;
				itemDesc = weapon_list[itemId].description;
				itemPrice = weapon_list[itemId].cost;
				break;
		}
		putString(1,i,32,24,D_NONE,itemName);
		putString(15,i,32,24,D_NONE,"$%d",itemPrice);
		if(i+start == selected)
			putString(0,20,31,3,D_NONE,itemDesc);

	}
}
