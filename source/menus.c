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

#define ITEMS_ON_SCREEN 19

extern u8 LETTER_WIDTH[];

/******************
 * drawMenu: Takes parameters and draws the menu with text inside
 * 
 * Return: -1 if B was pressed, otherwise return id of option selected.
*******************/
int drawMenu(int x, int y, int w, int h, char *text, int defValue) {
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
				tx = x;
				ty++;
				break;
			case '\1':
				choices[numChoices].x = tx;	// load x and y values of menu option
				choices[numChoices].y = ty;
				tx += 8;
				numChoices++;
				break;
			default:
				tx += LETTER_WIDTH[text[i]-' '];
				break;
		}
		i++;
	}

    drawTextBox(x, y, w, h, 0, D_NONE, text);

	// turn on cursor
	cursor->isHidden = false;

	int running = true;
	int selOption = defValue;
	int animation = 0;
	keysSetRepeat(16,5);

	// Handle key presses
	while(running) {

		animation++;
		cursor->x = choices[selOption].x+3+((animation & 0b10000)>>4);
		cursor->y = choices[selOption].y*8+4;
		oamUpdate(&oamSub);

		delay(1);

		scanKeys();
		int keys = keysDownRepeat();

		if((keys & KEY_DOWN || keys & KEY_RIGHT) && selOption < numChoices - 1) {
			selOption++;
		}
		else if (keysDown()&KEY_DOWN && selOption == numChoices-1) {
			selOption = 0;
		}
		if((keys & KEY_UP || keys & KEY_LEFT) && selOption > 0) {
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
	}
	// turn off cursor
	cursor->isHidden = true;
	oamUpdate(&oamSub);

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
extern char *menu_stats;
extern char *menu_options;
extern char *menu_save;
extern char *yes_or_no;

// item menu texts
extern char *menu_item_player_info;

// equipment texts
extern char *menu_equip_BL;


// store texts
extern char *store_items_txt;
extern char *store_buy_txt;
extern char *store_items_txt;

/***************
 * NEW GAME MENU
 ***************/
void menuNewGame() {

	putString(16,40,240,24,D_NONE,"New Game");
	putString(18,88,240,24,D_NONE,"Continue");

	bool running=true;
	Box boxes[2] = {{1,4,8,4},{1,10,8,4}};
	int selected = 0;
	while(running) {
		delay(1);
		drawBoxes(boxes,2,selected);
		scanKeys();
		if(keysDown()&KEY_A)
			running = false;
		if(keysDown()&KEY_DOWN || keysDown()&KEY_UP)
			selected = (selected+1)%2;

	}
}

/***************
 * MAIN MENU
 * ---------
 * Displays the main menu that pops up when you press Y
 * From here it extends into the other menus
 ***************/
void menuMain() {
	int selected=0;
	while(selected != -1) {
		clrSubScreen();
		selected = drawMenu(0,0,8,6,menu_Y,selected);		// draw the main menu
		clrSubScreen();
		switch(selected) {							// check which option was chosen
			case 0:
				menuItems();
				break;
			case 1:
				menuEquip();
				break;
			case 2:
				menuStats();
				break;
			case 3:
				drawTextBox(0,0,32,23,0,D_NONE,menu_options);
				waitAB();
				break;
			case 4:
				drawTextBox(0,0,32,23,0,D_NONE,menu_save);
				waitAB();
				break;
			default:
				selected = -1;
		}
		releaseKeys();
		clrSubScreen();
	}
}

/************************
 * ITEMS MENU
 ************************/
void menuItems() {
	dispItems();
	// load cursor
	SpriteEntry *cursor;
	cursor = initSprite(0, (u8 *)cursorTiles, cursorTilesLen);
	cursor->isHidden = false;

	int running = true;
	int selection = 0;
	int animation = 0;	// cursor animation
	keysSetRepeat(16,5);
	// Handle key presses
	while(running) {
		animation++;
		cursor->x = 3+((animation & 0b10000)>>4) + (selection < 19 ? 0 : 16*8);
		cursor->y = (selection < 19 ? selection : selection - 19)*8+4;
		oamUpdate(&oamSub);

		int itemId = party.inventory[selection].id;
		char *iDesc = "---";
		if(party.inventory[selection].amt > 0)
			iDesc = item_list[itemId].description;
		delText(0,160,248,24);
		putString(0,160,248,3,D_NONE,iDesc);

		delay(1);	// don't waste toooo much battery power!

		scanKeys();
		int keys = keysDownRepeat();

		if(keys & KEY_DOWN && selection < MAX_ITEMS - 1) {
			selection++;
		}
		else if (keysDown()&KEY_DOWN && selection == MAX_ITEMS-1) {
			selection = 0;
		}
		if(keys & KEY_UP && selection > 0) {
			selection--;
		}
		else if (keysDown()&KEY_UP && selection == 0) {
			selection = MAX_ITEMS-1;
		}
		if((keysDown()&KEY_RIGHT && selection < MAX_ITEMS/2) || (keysDown()&KEY_LEFT && selection < MAX_ITEMS/2)) {
			selection += 19;
		}
		else if((keysDown()&KEY_RIGHT) || (keysDown()&KEY_LEFT && selection >= MAX_ITEMS/2)) {
			selection -= 19;
		}

		if(keysDown() & KEY_B) {
			running = false;
		}
		if(keysDown() & KEY_A) {
			cursor->isHidden = true;
			oamUpdate(&oamSub);
			int player = 0;
			while(player >= 0) {
				player = selectCharacter(selection,player);
				if(player != -3)
					useItem(selection,player);
			}
			if(player > -2)
				dispItems();
			cursor->isHidden = false;
		}
	}
	// turn off cursor
	cursor->isHidden = true;
	oamUpdate(&oamSub);
}

void dispItems() {
	clrSubScreen();
	drawBox(0,0,16,20);	// main item box
	drawBox(16,0,16,20);	// main item box
	drawBox(0,20,32,4);	// item description box

	// display your items
	int i;
	for(i = 0; i < MAX_ITEMS; i++) {
		int itemId,itemAmt;
		char *iName;
		int y = i < 19 ? i : i-19;
		int x = i < 19 ? 0+8 : 16*8+8;
		itemId = party.inventory[i].id;
		itemAmt = party.inventory[i].amt;
		iName = item_list[itemId].name;
		if(itemAmt > 0) {
			putString(x,y*8,32,24,D_NONE,iName);
			putString(x+71,y*8,32,24,D_NONE,"*%d",itemAmt);
		} else {
			putString(x,y*8,32,24,D_NONE,"---");
		}
	}
}

/**********************
 * SELECT CHARACTER
 * ----------------
 * Player chooses which character to give the item to
 **********************/
int selectCharacter(int itemPos, int selected) {
	if(party.inventory[itemPos].amt == 0)
		return -3;

	int itemId = party.inventory[itemPos].id;
	if(item_list[itemId].type == IT_OTHER)
		return -2;		// this isn't an item to be used on a character (eg. repel, escape rope, ...)

	clrSubScreen();

	int i;
	Box boxes[3];
	for(i = 0; i < 3; i++) {
		boxes[i].x = 0;
		boxes[i].y = i*8;
		boxes[i].w = 32;
		boxes[i].h = 8;
	}
	drawBoxes(boxes, 3, -1);

	for(i = 0; i < party.numMembers; i++) {
		char *name;
		int hp_max,hp;
		name = party.member[i].name;
		hp = party.member[i].hp;
		hp_max = party.member[i].hp_max;
		putString(8,i*8*8,240,8,D_NONE,menu_item_player_info,name,hp,hp_max);
	}

	// load cursor
	SpriteEntry *cursor;
	cursor = initSprite(0, (u8 *)cursorTiles, cursorTilesLen);
	cursor->isHidden = false;

	int itemType = item_list[itemId].type;
	bool running = true;
	keysSetRepeat(16,5);
	int animation = 0;	// cursor animation
	while(running) {
		cursor->x = 3+((animation++ & 0b10000)>>4);
		if(itemType == IT_ALL)
			cursor->y = 8*8*((animation>>1)%party.numMembers)+4+3*8;
		else
			cursor->y = 8*8*selected+4+3*8;
		oamUpdate(&oamSub);
		delay(1);

		if(itemType == IT_ALL)
			drawBoxes(boxes, party.numMembers, 255);
		else
			drawBoxes(boxes, party.numMembers, selected);
		scanKeys();
		int keys = keysDownRepeat();
		if (keys & KEY_DOWN && selected < party.numMembers-1) {
			selected++;
		}
		if (keys & KEY_UP && selected > 0) {
			selected--;
		}
		if (keysDown() & KEY_B) {
			selected = -1;
			running = false;
		}
		if (keysDown() & KEY_A)
			running = false;
	}
	return selected;
}

/******************
 * EQUIPMENT
 ******************/

void menuEquip() {
	clrSubScreen();
	drawBox(0,0,16,4);	// party members
	drawBox(0,4,16,20);	// equiped weapons
	drawBox(16,0,16,24);	// unequipped weapons
	int i;
	for(i=0;i<3;i++)
		putString(8,i*8,248,24,D_NONE,party.member[i].name);

	SpriteEntry *cursor;
	cursor = initSprite(0, (u8 *)cursorTiles, cursorTilesLen);
	cursor->isHidden = false;

	int selPlayer = 0;
	int animation = 0;
	bool running = true;
	while(running) {
		delay(1);
		cursor->x = 3+((animation++ & 0b10000)>>4);
		cursor->y = 4+selPlayer*8;
		oamUpdate(&oamSub);

		scanKeys();
		if(keysDown()& KEY_DOWN && selPlayer < 2 && party.member[selPlayer+1].active) {
			selPlayer++;
		}
		if(keysDown()& KEY_UP && selPlayer > 0) {
			selPlayer--;
		}
		if(keysDown()&KEY_B)
			running = false;
		if(keysDown()&KEY_A) {
			viewEquip(selPlayer);
		}
	}
	// turn off cursor
	cursor->isHidden = true;
	oamUpdate(&oamSub);
}

void viewEquip(int pId) {
	char *weapon_txt, *armor_txt;
	weapon_txt = weapon_list[party.member[pId].wId].name;
	armor_txt = armor_list[party.member[pId].aId].name;
	putString(0,4*8,31*8,24,D_NONE,menu_equip_BL,weapon_txt,armor_txt);

	MenuChoice cursCoords[2] = {{3,5*8+4},{3,8*8+4}};

	SpriteEntry *cursor;
	cursor = initSprite(1, (u8 *)cursorTiles, cursorTilesLen);
	cursor->isHidden = false;

	bool running = true;
	int selection = 0;
	int animation = 0;
	while(running) {
		delay(1);
		cursor->x = cursCoords[selection].x+((animation++ & 0b10000)>>4);
		cursor->y = cursCoords[selection].y;
		oamUpdate(&oamSub);

		drawEquip(0,pId,selection);

		scanKeys();
		if(keysDown()& KEY_DOWN && selection < 1) {
			selection++;
		}
		if(keysDown()& KEY_UP && selection > 0) {
			selection--;
		}
		if(keysDown()&KEY_B)
			running = false;
		if(keysDown()&KEY_A) {
			chooseEquip(pId,selection);
			// update weapon and armor text since we may have changed equipment
			delText(0,4*8,15*8,23*8);
			weapon_txt = weapon_list[party.member[pId].wId].name;
			armor_txt = armor_list[party.member[pId].aId].name;
			putString(0,32,31*8,23,D_NONE,menu_equip_BL,weapon_txt,armor_txt);
		}
	}
	delText(0,4*8,15*8,19*8);
	delText(16*8,0,15*8,23*8);
	// turn off cursor
	cursor->isHidden = true;
	oamUpdate(&oamSub);
}

void chooseEquip(int pId, int eType) {
	SpriteEntry *cursor;
	cursor = initSprite(2, (u8 *)cursorTiles, cursorTilesLen);
	cursor->isHidden = false;

	bool running = true;
	int selection = 0;
	int offset = 0;
	int animation = 0;
	while(running) {
		delay(1);
		cursor->x = 16*8 + 3+((animation++ & 0b10000)>>4);
		cursor->y = (selection-offset)*8 + 4;
		oamUpdate(&oamSub);

		drawEquip(0,pId,eType);

		scanKeys();
		int keys = keysDownRepeat();
		if(keys& KEY_DOWN && selection < 19) {
			selection++;
		}
		if(keys& KEY_UP && selection > 0) {
			selection--;
		}
		if(keysDown()&KEY_B)
			running = false;
		if(keysDown()&KEY_A) {
			if(eType == 0) {
				// swap weapons
				int curWeapon = party.member[pId].wId;
				int newWeapon = party.weapons[selection+offset];
				if(party.weapons[selection+offset] == 0)
					newWeapon = 0;
				party.weapons[selection+offset] = curWeapon;
				party.member[pId].wId = newWeapon;
			} else {
				// swap armor
				int curArmor = party.member[pId].aId;
				int newArmor = party.armor[selection+offset];
				if(party.armor[selection+offset] == 0)
					newArmor = 0;
				party.armor[selection+offset] = curArmor;
				party.member[pId].aId = newArmor;
			}
			running = false;
		}
	}
	// turn off cursor
	cursor->isHidden = true;
	oamUpdate(&oamSub);
}

void drawEquip(int offset, int pId, int eType) {
	// NOTE: We can use pId to check if the player can use that kind of weapon/armor
	delText(16*8,0,16*8,23*8);
	char *empty = "---";
	char *name;
	int i;
	for(i = 0; i < 20; i++) {
		name = empty;
		if(eType == 0) {
			if(party.weapons[i+offset] > 0)
				name = weapon_list[party.weapons[i+offset]].name;
		} else {
			if(party.armor[i+offset] > 0)
				name = armor_list[party.armor[i+offset]].name;
		}
		putString(17*8,i*8,31*8,24,D_NONE,name);
	}
}


/*****************
 * STATS
 *****************/
void menuStats() {
	drawBox(0,0,32,24);
	char *name, *weapon, *armor;
	int hp,hp_max,weapon_str,str,armor_def,def,agi;
	int i = 0;
	bool running = true;
	while(running) {
		name = party.member[i].name;
		hp = party.member[i].hp;
		hp_max = party.member[i].hp_max;
		weapon_str = weapon_list[party.member[i].wId].atk;
		str = party.member[i].str;
		armor_def = armor_list[party.member[i].aId].def;
		def = party.member[i].def;
		agi = party.member[i].agi;
		weapon = weapon_list[party.member[i].wId].name;
		armor = armor_list[party.member[i].aId].name;

		// display the stats screen with the stats inserted into it
		putString(0,0,248,23,D_NONE,menu_stats,name,hp,hp_max,str+weapon_str,str,armor_def+def,def,agi,weapon,armor);

		int keys = waitKey();

		if (keys & KEY_RIGHT && party.member[i+1].active && i < 2) {
			delText(0,0,31*8,23*8);
			i++;
		}
		if (keys & KEY_LEFT && i > 0) {
			delText(0,0,31*8,23*8);
			i--;
		}
		if (keys & KEY_B)
			running = false;
	}
}

/*********************
 * SHOPS
 *********************/
void menuStore(int id) {
	// display the text!
	drawTextBox(0,0,32,2,0,D_SLOW,store_list[id].hello);
	waitAB();				// wait for player to press [A] or [B]
	clrSubScreen();


	drawBox(0,0,18,20);		// main item box
	drawBox(18,0,14,20);	// extra info/purchase box
	drawBox(0,20,32,4);		// item description box

	// load cursor
	SpriteEntry *cursor;
	cursor = initSprite(1, (u8 *)cursorTiles, cursorTilesLen);
	cursor->isHidden = false;

	int running = true;
	int selOption = 0;
	int topOption = 0;
	int numItems = store_list[id].numItems;
	int itemsOnScreen = ITEMS_ON_SCREEN;
	if(itemsOnScreen > numItems)
		itemsOnScreen = numItems;
	int animation = 0;
	keysSetRepeat(16,5);
	// display the store
	drawStoreItems(id,topOption,selOption);
	// Handle key presses
	while(running) {
		animation++;
		cursor->x = 3 + ((animation & 0b10000)>>4);
		cursor->y = (selOption-topOption)*8+4;
		oamUpdate(&oamSub);

		delay(1);
		drawItemInfo(id,topOption,selOption);

		scanKeys();
		int keys = keysDownRepeat();

		if(keys & KEY_DOWN && selOption < numItems - 1) {
			selOption++;
			if((selOption > topOption + itemsOnScreen-1) && (topOption + itemsOnScreen < numItems)) {
				topOption++;
				drawStoreItems(id,topOption,selOption);
			}
		}
		else if (keysDown()&KEY_DOWN && selOption == numItems-1) {
			topOption = 0;
			selOption = 0;
			drawStoreItems(id,topOption,selOption);
		}
		if(keys & KEY_UP && selOption > 0) {
			selOption--;
			if((selOption < topOption) && (topOption > 0)) {
				topOption--;
				drawStoreItems(id,topOption,selOption);
			}
		}
		else if (keysDown()&KEY_UP && selOption == 0) {
			topOption = numItems-itemsOnScreen;
			selOption = numItems-1;
			drawStoreItems(id,topOption,selOption);
		}

		if(keysDown()&KEY_A) {
			// buy item
			buyItem(id, selOption);
			scanKeys();
		}
		if(keysDown()&KEY_B) {
			// exit store
			running = false;
		}
	}
	// turn off cursor
	cursor->isHidden = true;
	oamUpdate(&oamSub);

	// display the text!
	delText(0,160,256,24);
	drawTextBox(0,20,32,3,0,D_SLOW,store_list[id].goodbye);
	waitAB();				// wait for player to press [A] or [B]
	clrSubScreen();

	releaseKeys();

}

/***************
 * BUY ITEM
 ***************/
void buyItem(int storeId, int selected) {
	int amt = 0;
	int itemPrice = 0;
	int itemType = store_list[storeId].items[(selected)*2];
	int itemId = store_list[storeId].items[(selected)*2+1];
	int curAmt = 0;
	int max = 99;
	if(itemType == I_ITEM) {
		int itemPos = findItemPos(itemId);
		if(itemPos != -1)
			curAmt = party.inventory[itemPos].amt;
	} else {
		curAmt = countEquip(itemType,-1);
		max = 20;
	}
	switch(itemType) {
		case 0:		// item
			itemPrice = item_list[itemId].cost;
			break;
		case 1:		// armor
			itemPrice = armor_list[itemId].cost;
			break;
		case 2:		// weapon
			itemPrice = weapon_list[itemId].cost;
			break;
	}
	keysSetRepeat(16,5);
	bool running = true;
	while(running) {
		delay(1);
		delText(144,48,112,14*8);
		putString(144,48,248,24,D_NONE,store_buy_txt,amt,amt*itemPrice);

		scanKeys();
		int keys = keysDownRepeat();

		if(keys & KEY_RIGHT && amt < max-curAmt && party.gold >= (amt+1)*itemPrice)
			amt++;
		if(keys & KEY_LEFT && amt > 0)
			amt--;

		if(keysDown()&KEY_A) {
			// confirm purchase
			if(amt > 0) {
				int choice = drawMenu(0, 20, 32, 3, yes_or_no, 0);
				if(choice == 0) {
					if(receiveItem(itemType,itemId,amt))
						party.gold -= amt*itemPrice;
					else
						drawTextBox(0,0,32,24,0,D_NONE,"ERROR!");
				}
			}
			running = false;
		}
		if(keysDown()&KEY_B) {
			// exit store
			running = false;
		}
	}
		
}

void drawItemInfo(int storeId, int start, int selected) {
	int itemType = store_list[storeId].items[selected*2];
	int itemId = store_list[storeId].items[selected*2+1];
	char *itemDesc = "";
	switch(itemType) {
		case 0:		// item
			itemDesc = item_list[itemId].description;
			break;
		case 1:		// armor
			itemDesc = armor_list[itemId].description;
			break;
		case 2:		// weapon
			itemDesc = weapon_list[itemId].description;
			break;
	}
	delText(0,160,248,24);
	putString(0,160,248,3,D_NONE,itemDesc);

	int curAmt = 0;
	if(itemType == I_ITEM) {
		int itemPos = findItemPos(itemId);
		if(itemPos != -1)
			curAmt = party.inventory[itemPos].amt;
	} else
		curAmt = countEquip(itemType,itemId);
	delText(144,0,100,100);
	putString(144,0,100,23,D_NONE,store_items_txt,party.gold,curAmt);
}

void drawStoreItems(int storeId, int start, int selected) {
	delay(1);
	delText(0,0,140,154);
	int i,numItems;
	numItems = store_list[storeId].numItems;
	if(numItems + start > ITEMS_ON_SCREEN)	// make sure we don't try to draw too many items
		numItems = ITEMS_ON_SCREEN;
	for(i = 0; i < numItems; i++) {
		int itemType = store_list[storeId].items[(i+start)*2];
		int itemId = store_list[storeId].items[(i+start)*2+1];
		int itemPrice = 0;
		char *itemName = "";

		switch(itemType) {
			case 0:		// item
				itemName = item_list[itemId].name;
				itemPrice = item_list[itemId].cost;
				break;
			case 1:		// armor
				itemName = armor_list[itemId].name;
				itemPrice = armor_list[itemId].cost;
				break;
			case 2:		// weapon
				itemName = weapon_list[itemId].name;
				itemPrice = weapon_list[itemId].cost;
				break;
		}
		putString(8,i*8,256,24,D_NONE,itemName);
		putString(88,i*8,256,24,D_NONE,":");
		putString(17*8-intWidth(itemPrice),i*8,256,24,D_NONE,"%d",itemPrice);
	}
}
