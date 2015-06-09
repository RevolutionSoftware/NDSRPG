#include <nds.h>	// Main NDS equates

#include "utilities.h"
#include "text.h"
#include "font.h"
#include "player.h"
#include "weapons.h"
#include "armor.h"

#define TL	'~'-' '+1	// top left (this is the tile id)
#define T	TL+1		// top
#define TR	T+1			// top right
#define BL	TR+1		// bottom left
#define B	BL+1		//
#define BR	B+1			//
#define L	BR+1		// left
#define R	L+1			//
#define C	R+1			// center

/*************
 * Performs word wrapping
 * - pass pointers to x and y and function updates them for you
 *************/
void wordWrap(int *x, int *y, int r_edge, int l_edge, const char *text) {
	int i=0;
	int j = i+1;
	while(text[j] != ' ' && text[j] > '\1' && text[j] != '\n') {
		j++;
	}
	if(j-i+*x > r_edge) {	// j-i = number of characters to next space
								// .. x + characters to next space
		*x = l_edge;			// move to a new line
		*y += 1;
	}
}

// text is defined as a tilemap.
void putString(int x, int y, int w, e_speed flag, const char *text, ...) {
	// get pointer to arg list
	int *argp;
	char c;
	argp = (int *)&text;
	argp++;

	// Draw the message on the screen.
	int text_length = stringLength(text);
	int left_edge = x;
	int right_edge = left_edge + w;
	int i;

	for (i = 0; i < text_length; i++) {
		c = *text++;
		// Check for special characters (\n, etc.)
		switch(c) {
			case '\n':
				x = left_edge;
				y++;
				break;
			case '%':
				c = *text++;
				i++;
				switch(c) {
					case 'd':			// display the int
						putInt(&x,y,flag,*argp++);
						break;
					case 's':			// display the string
						wordWrap(&x, &y, right_edge, left_edge, text);
						char *str = (char *)*argp++;
						putString(x,y,w,flag,str);
						x += stringLength(str);
						break;
				}
				break;
			case ' ':
			case '\1':					// \1 is the menu option character
				x++;					// shift over one character for the space
				wordWrap(&x, &y, right_edge, left_edge, text);
				break;
			default:
				putChar(&x,y,flag,c-' ');
		}
    }
}

/*****************
 * converts integer to string
 *****************/
void putInt(int *x, int y, e_speed flag, int num) {
	int i = num/10;
	if(i>0)
		putInt(x,y,flag,i);
	putChar(x,y,flag,num%10 + '0'-' ');
}

void putChar(int *x, int y, e_speed flag, char c) {
	u16 *sub_map = BG_MAP_RAM_SUB(0);		// address of text tilemap
	sub_map[y*32+*x] = c;					// draw the character to the tilemap
	*x += 1;								// advance x
	delay(flag);
}	

int stringHeight(const char *text) {
	int i,h;
	i = 0;
	h = 1;
	while(text[i] != '\0') {
		if(text[i] == '\n')
			h++;
		i++;
	}
	return h;
}

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

// draws a box with text inside it
void drawTextBox(int x, int y, int w, int h, const char *text, e_speed flag) {
    drawBox(x,y,w,h+1);
    putString(x,y,w-1,flag,text);
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

void clrSubScreen() {
	delTextBox(0,0,32,24);
}
