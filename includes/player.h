#ifndef PLAYER_H
#define PLAYER_H

typedef struct {
	char name[11];

	int lvl;
	int exp;

// stats
	int hp_max;
	int hp;				// current hp (ie. not the max hp)
	int str;
	int def;
	int agi;

// equipment
	int wId;			// NOTE: u8 is probably more than enough, but will it actually save space?
	int aId;			// same as above

// misc
	int atkBarLen;
	int state;			// poison, paralyze, etc.
	bool active;		// if the character is in the party or not
}Character;

#define MAX_ITEMS	38

typedef struct {
	int id;
	int amt;
}Inventory;

typedef struct {
	Character member[3];
	Inventory inventory[MAX_ITEMS];
	int weapons[20];
	int armor[20];
	int gold;
	int numMembers;
}Party;

Party party;

bool addHP(int pId, int amt);
int getAtk(int pId);

#endif // PLAYER_H
