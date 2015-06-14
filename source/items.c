#include <nds.h>
#include "items.h"
#include "player.h"

extern Party party;

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
