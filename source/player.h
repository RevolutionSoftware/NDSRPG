#ifndef PLAYER_H
#define PLAYER_H

#define PLAYER_WIDTH 17
#define PLAYER_HEIGHT 24
#define SPEED 2
#define ANIMATION_SPEED 8

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
	int wId;		// NOTE: u8 is probably more than enough, but will it actually save space?
	int aId;		// same as above

// misc
	int state;			// poison, paralyze, etc.
	bool active;		// if the character is in the party or not
}Character;

Character party[3];


#endif // PLAYER_H
