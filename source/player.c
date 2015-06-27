#include <nds.h>
#include "player.h"
#include "weapons.h"

extern Party party;
extern Weapon weapon_list[];

bool addHP(int pId, int amt) {
	if(party.member[pId].hp < party.member[pId].hp_max) {
		party.member[pId].hp += amt;
		if(party.member[pId].hp > party.member[pId].hp_max)
			party.member[pId].hp = party.member[pId].hp_max;
		return true;
	} else
		return false;
}

int getAtk(int pId) {
	int wpn = party.member[pId].wId;
	int atk = party.member[pId].str;
	atk += weapon_list[wpn].atk;
	return atk;
}
