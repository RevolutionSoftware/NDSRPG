#include <nds.h>	// Main NDS equates

#include "aux_macros.h"
#include "text.h"
#include "font.h"

int stringLength(const char *str) {
    int string_length = 0;
    while (str[string_length] != '\0') {
        string_length++;
    }
    return string_length;
}

// text is defined as a tilemap
void putString(const char *text) {
    int i;

    //int text_length = stringLength(text);
    // get size of string (sizeof won't work for pointers)

    // address of tilemap
    u16 *sub_map = BG_MAP_RAM_SUB(0);

    // Draw the message on the screen.
    int x = 0, y = 0;

    for (i = 0; i < text_length; i++) {
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
// String Exploder
char *stringExploder(char *str, const char* delim) {
    local_persist char* _buffer;

    if(str != NULL) {
        _buffer = str;
    }
    if(_buffer[0] == '\0') {
        return NULL;
    }

    char *ret = _buffer, *b;
    const char *d;

    for(b = _buffer; *b !='\0'; b++) {
        for(d = delim; *d != '\0'; d++) {
            if(*b == *d) {
                *b = '\0';
                _buffer = b+1;

                if(b == ret) {
                    ret++;
                    continue;
                }
                return ret;
            }
        }
    }
    return ret;
}
