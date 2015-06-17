#include <nds.h>
#include <time.h>
#include "tilemap.h"
#include "items.h"
#include "player.h"

extern Drawable player;
extern map_t Level;
extern Party party;


void delay (u32 sleep_time) {
    u32 i = 0;
    while (i < sleep_time) {
        swiWaitForVBlank();
        i++;
    }
}

void releaseKeys() {
	int keys = keysCurrent();
	while((keys & 0b111111111111) != 0) {
		keys = keysCurrent();
		delay(1);
	}
}

int waitKey() {
	// first release the keys
	releaseKeys();

	scanKeys();

	int keys = keysDown();
	
	while((keys & 0b111111111111) == 0) {
		scanKeys();
		keys = keysDown();
		delay(1);
	}
	return keys;
}

// waits for the user to press A or B
int waitAB() {
	int keys;
	do {
		keys = waitKey();
	} while(!(keys & KEY_A) && !(keys & KEY_B));
	return keys;
}

int randNum(int max) {
	static u16 seed = 0x1337;
	unsigned bit;
	u32 i = time(NULL);

	seed += i;

	bit  = ((seed >> 0) ^ (seed >> 2) ^ (seed >> 3) ^ (seed >> 5) ) & 1;
	seed =  (seed >> 1) | (bit << 15);

	return seed % max;
}
