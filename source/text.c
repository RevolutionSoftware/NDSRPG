#include <nds.h>	// Main NDS equates

#ifdef USE_STRLEN
#include <string.h> // strlen()
#endif // USE_STRLEN

#include "text.h"

// text is defined as a tilemap
void putString(char *text) {
    int i;

#ifndef USE_STRLEN
    int size = 0;
    // get size of string (sizeof won't work for pointers)
    while(text[size] != '\0') {
       size++;
    }
#endif

#ifdef USE_STRLEN
    size_t text_length = strlen(text); // string.h
#endif // USE_STRLEN

    // address of tilemap
    u16 *sub_map = BG_MAP_RAM_SUB(0);

    // Draw the message on the screen.
    int x = 0, y = 0;

#ifdef USE_STRLEN
    for (i = 0; i < text_length; i++) {
#else
    for(i = 0; i < size; i++) {
#endif // USE_STRLEN

        // Check for special characters (\n, etc.)
        if (text[i] == '\n') {
            x = 0;
            y++;
        }
        else if (text[i] == ' ') {
            int j = i++;

            while(text[j] != ' ' && text[j] != '\0') {
                j++;
            }
            if(j-i+x > 32) {		// j-i = number of characters to next space
                                        // .. x + characters to next space
                i++;			// skip the space
                x = 0;			// and move to a new line
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
