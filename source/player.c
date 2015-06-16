#include <nds.h>
#include "player.h"

extern Party party;

bool addHP(int pId, int amt) {
	if(party.member[pId].hp < party.member[pId].hp_max) {
		party.member[pId].hp += amt;
		if(party.member[pId].hp > party.member[pId].hp_max)
			party.member[pId].hp = party.member[pId].hp_max;
		return true;
	} else
		return false;
}
