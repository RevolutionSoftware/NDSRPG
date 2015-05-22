#include <nds.h>	// Main NDS equates
#include "font.h"

// text is defined as a tilemap
void putString(char *text) {
	int i,size=0;

	// get size of string (sizeof won't work for pointers)
	while(text[size] != '\0')
		size++;

	// address of tilemap
	u16 *sub_map = BG_MAP_RAM_SUB(0);

	// Draw the message on the screen.
	int x,y;
	x = 0;
	y = 0;
	for(i = 0; i < size; i++) {
		// Check for special characters (\n, etc.)
		switch(text[i]) {
			case '\n':
				x = 0;
				y++;
				continue;			// *is this bad practice?*
		}
		// text wrapping
		if(text[i] == ' ') {
			int j = i+1;
			while(text[j] != ' ' && text[j] != '\0') {
				j++;
			}
			if(j-i+x > 32) {		// j-i = number of characters to next space
									// .. x + characters to next space
				i++;				// skip the space
				x = 0;				// and move to a new line
				y++;
			}
		}
		// copy tile to map
		sub_map[y*32+x] = text[i]-' ';
		x++;
		// Check for new line
		x %= 32;	// screen is 32 tiles wide
		if(!x)
			y++;
	}
}
