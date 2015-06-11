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
	int state;			// poison, paralyze, etc.
	bool active;		// if the character is in the party or not
}Character;

typedef struct {
	Character member[3];
	int gold;
}Party;

Party party;


#endif // PLAYER_H
