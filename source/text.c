#include <nds.h>	// Main NDS equates

#include "utilities.h"
#include "text.h"
#include "font.h"


int stringLength(const char *text) {
    int text_length = 0;
    while(text[text_length] != '\0') {
        text_length++;
    }
    return text_length;
}

// text is defined as a tilemap.
// if speed is 0, no delay is used.
void putString(const char *text, unsigned int flag) {
    int text_length = stringLength(text);
    // address of tilemap
    u16 *sub_map = BG_MAP_RAM_SUB(0);

    // Draw the message on the screen.
    int x = 0, y = 0;
    int i;

    if (flag > SLOW) {
        flag = SLOW;
    }

    for (i = 0; i < text_length; i++) {
        delay(flag);
        // Check for special characters (\n, etc.)
        if (text[i] == '\n') {
            x = 0;
            y++;
            continue;
        }
        else if (text[i] == ' ') {
            int j = i+1;

            while(text[j] != ' ' && text[j] != '\0') {
                j++;
            }
            if(j-i+x > 32) {	// j-i = number of characters to next space
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
