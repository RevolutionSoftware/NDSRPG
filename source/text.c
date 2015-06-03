#include <nds.h>	// Main NDS equates

#include "utilities.h"
#include "text.h"
#include "font.h"

#define TL	'~'-' '+1	// top left
#define T	TL+1		// top
#define TR	T+1			// top right
#define BL	TR+1		// bottom left
#define B	BL+1		//
#define BR	B+1			//
#define L	BR+1		// left
#define R	L+1			//
#define C	R+1			// center

// text is defined as a tilemap.
void putString(int x, int y, int w, const char *text, e_speed flag) {
    int text_length = stringLength(text);
    // address of tilemap
    u16 *sub_map = BG_MAP_RAM_SUB(0);

    // Draw the message on the screen.
    int left_edge = x;
    int right_edge = left_edge + w;
    int i;

    if (flag > D_SLOW) {
        flag = D_SLOW;
    }

    for (i = 0; i < text_length; i++) {
        delay(flag);
        // Check for special characters (\n, etc.)
        if (text[i] == '\n') {
            x = left_edge;
            y++;
        }
        else {
            if (text[i] == ' ') {
                int j = i+1;

                while(text[j] != ' ' && text[j] != '\0') {
                    j++;
                }
                if(j-i+x > right_edge) {	// j-i = number of characters to next space
                    // .. x + characters to next space
                    i++;					// skip the space
                    x = left_edge;			// and move to a new line
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
}
/*
  How wide/high does the box have to be?
  Where do we want it aligned (centered? To the left or right?)
  Some good standard alignment values:
  ------------ Down - Central ------------
  Xstart = Xmin + 20pix -> Xmax - 20pix
  Ystart = Ymax - 20pix -> Ymax
  ----------- Middle - Central -----------
  X.1 = Xstart \ Y.1 = Ystart
  X.2 = Xend   \ Y.2 = Yend

  Xd (delta) = X.2 - X.1
  Yd (delta) = Y.2 - Y.1

  X.1 = (Xmax/2)-Zpixels
  X.2 = (Xmax/2)+Zpixels
  Y.1 = (Ymax/2)-Ipixels
  Y.2 = (Ymax/2)+Ipixels
  ----------------------------------------
*/

void drawBox(int x, int y, int w, int h) {
    delTextBox(x,y,w,h);
    u16 *box_map = BG_MAP_RAM_SUB(1);
    int i,j;
    box_map += 32*y+x;		// map width is 32
    // draw top row
    *box_map = TL;
    for(i = 0; i < w-2; i++) {
        box_map++;
        *box_map = T;
    }
    box_map++;
    *box_map = TR;
    box_map += 33-w;		// move to next line
    // draw middle area
    for(j = 0; j < h-2; j++) {
        *box_map = L;
        for(i = 0; i < w-2; i++) {
            box_map++;
            *box_map = C;
        }
        box_map++;
        *box_map = R;
        box_map += 33-w;	// move to next line
    }
    // draw bottom row
    *box_map = BL;
    for(i = 0; i < w-2; i++) {
        box_map++;
        *box_map = B;
    }
    box_map++;
    *box_map = BR;
}

void drawTextBox(int x, int y, int w, int h, const char *text, e_speed flag) {
    drawBox(x,y,w,h+1);
    putString(x,y,w-1,text,flag);
}

void delTextBox(int x, int y, int w, int h) {
    u16 *text_map = BG_MAP_RAM_SUB(0)+32*y+x;
    u16 *box_map = BG_MAP_RAM_SUB(1)+32*y+x;
    int i,j;

    // erase box by overwriting it with 0s
    for(j = 0; j < h; j++) {
        for(i = 0; i < w; i++) {
            *text_map = 0;
            text_map++;
            *box_map = 0;
            box_map++;
        }
        text_map += 32-w;
        box_map += 32-w;	// move to next line
    }
}
