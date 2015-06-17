#include <nds.h>
#include "items.h"
#include "player.h"
#include "text.h"
#include "utilities.h"

// sprites
#include "cursor.h"

extern Party party;
extern Item item_list[];

int findItemPos(int id) {
	int j;
	for(j = 0; j < MAX_ITEMS; j++) {
		if(party.inventory[j].id == id && party.inventory[j].amt > 0)
			break;
	}
	return j < MAX_ITEMS ? j : -1;
}

int findItemEmpty() {
	int j;
	for(j = 0; j < MAX_ITEMS; j++) {
		if(party.inventory[j].amt == 0)
			break;
	}
	return j < MAX_ITEMS ? j : -1;
}

bool receiveItem(int itemType, int itemId, int amt) {
	if(itemType == I_ITEM) {
		int itemPos = findItemPos(itemId);
		if(itemPos == -1)
			itemPos = findItemEmpty();
		if(itemPos != -1) {
			party.inventory[itemPos].id = itemId;
			party.inventory[itemPos].amt += amt;
			return true;
		} else
			return false;
	}
	else {
		int j;
		for(j=0;j<amt;j++) {
			if(itemType == I_WEAPON) {
				int i;
				for(i = 0; i<20; i++)
					if(party.weapons[i] == 0) {
						party.weapons[i] = itemId;
						break;
					}
			}
			else if(itemType == I_ARMOR) {
				int i;
				for(i = 0; i<20; i++)
					if(party.armor[i] == 0) {
						party.armor[i] = itemId;
						break;
					}
			}
		}
	}
	return true;

}

/***********************
 * COUNT EQUIPMENT
 * ---------------
 * counts how many of a certain weapon/armor you have
 * Input:
 * -item's type (armor or weapon)
 * -item's id
 * Output:
 * -number of that item in player's inventory
 ***********************/
int countEquip(int itemType, int itemId) {
	int i;
	int count=0;
	if(itemType == I_WEAPON) {
		for(i=0;i<20;i++)
			if(party.weapons[i] == itemId || (itemId == -1 && party.weapons[i] > 0))
				count++;
	} else {
		for(i=0;i<20;i++)
			if(party.armor[i] == itemId || (itemId == -1 && party.armor[i] > 0))
				count++;
	}
	return count;
}


/**********************
 * USE ITEM
 * --------
 * Let's player use an item from their inventory
 **********************/
void useItem(int itemPos, int pId) {
	int itemId = party.inventory[itemPos].id;
	switch(itemId) {
		case I_BANANA:	// heals 10 hp for one player
			if(addHP(pId,10))
				party.inventory[itemPos].amt--;
			break;
		case I_OATMEAL:	// heals 20 hp for one player
			if(addHP(pId,20))
				party.inventory[itemPos].amt--;
			break;
		case I_TENT:	// heals all hp for all players
			if(addHP(0,9999) && addHP(1,9999) && addHP(2,9999))
				party.inventory[itemPos].amt--;
			break;
		case I_FLOWER:	// i dunno what this does yet
			break;
	}
}
