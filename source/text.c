#include <nds.h>	// Main NDS equates

#include "utilities.h"
#include "text.h"
#include "font.h"
#include "player.h"
#include "weapons.h"
#include "armor.h"
#include "objects.h"
#include "menus.h"

// sprites
#include "cursor.h"

#define TL	'~'-' '+1	// top left (this is the tile id)
#define T	TL+1		// top
#define TR	T+1			// top right
#define BL	TR+1		// bottom left
#define B	BL+1		//
#define BR	B+1			//
#define L	BR+1		// left
#define R	L+1			//
#define C	R+1			// center

#define BOX_TILES 9

//					   ! " # $ % & ' ( ) * + , - . /
u8 LETTER_WIDTH[] = {2,2,4,6,6,8,6,2,3,3,4,6,3,5,2,6,
//					0 1 2 3 4 5 6 7 8 9 : ; < = > ?
					5,4,5,5,6,5,5,5,6,5,6,6,4,4,4,6,
//					@ A B C D E F G H I J K L M N O
					8,6,6,6,6,6,6,6,6,4,5,6,5,6,6,6,
//					P Q R S T U V W X Y Z [ \ ] ^ _
					6,6,6,6,6,6,6,8,6,6,6,3,6,3,4,6,
//					` a b c d e f g h i j k l m n o
					3,5,5,6,5,5,5,5,5,2,4,5,3,6,5,5,
//					p q r s t u v w x y z { | } ~
					5,5,5,5,5,5,5,6,5,5,5,4,2,4,5};

/*************
 * Performs word wrapping
 * - pass pointers to x and y and function updates them for you
 *************/
void wordWrap(int *x, int *y, int r_edge, int l_edge, const char *text) {
	int i=0;
	int width=0;
	while(text[i] != ' ' && text[i] > '\1' && text[i] != '\n') {
		width += LETTER_WIDTH[(int) text[i]-' '];
		i++;
	}
	if(width+*x > r_edge) {	// j-i = number of characters to next space
							// .. x + characters to next space
		*x = l_edge;		// move to a new line
		*y += 8;
	}
}

// text is defined as a tilemap.
void putString(int x, int y, int w, int h, e_speed flag, const char *text, ...) {
	x+=4;		// normally there will be a border around the text, this avoids having to add 4 each time
	y+=4;
	// get pointer to arg list
	int *argp;
	char c;
	argp = (int *)&text;
	argp++;

	// Draw the message on the screen.
	int text_top = y;
	int text_length = stringLength(text);
	int left_edge = x;
	int right_edge = left_edge + w;
	int i;

	for (i = 0; i < text_length; i++) {
		// check if text goes beyond bottom of the textbox
		if(y >= text_top+h*8) {
			// load little blinky thingy telling you there's more text to read
			SpriteEntry *blinky;
			blinky = initSprite(0, (u8 *)cursorTiles+64, cursorTilesLen);
			blinky->isHidden = false;
			blinky->x = left_edge+w-7;
			blinky->y = text_top+h*8-4;

			int counter = 0b000000;
			scanKeys();
			while(!(keysDown()&KEY_A) && !(keysDown()&KEY_B)) {
				counter++;
				if(counter & 0b100000)
					blinky->isHidden = true;
				else
					blinky->isHidden = false;
				oamUpdate(&oamSub);
				scanKeys();
				delay(1);
			}

			blinky->isHidden = true;
			oamUpdate(&oamSub);
			releaseKeys();
			delText(left_edge-4, text_top-4, w, h*8);
			x = left_edge;
			y = text_top;
		}
		
		c = *text++;
		// Check for special characters (\n, etc.)
		switch(c) {
			case '\n':
				x = left_edge;
				y += 8;
				break;
			case '%':
				c = *text++;
				i++;					// we also need to update the counter
				switch(c) {
					case 'd':			// display the int
						x = putInt(x,y,flag,*argp++);
						break;
					case 's':			// display the string
						wordWrap(&x, &y, right_edge, left_edge, text);
						char *str = (char *)*argp++;
						putString(x-4,y-4,w,h,flag,str);
						x += stringWidth(str);
						break;
				}
				break;
			case '\2':					// change x position
				x = 4 + *text++;
				break;
			case '\1':					// \1 is the menu option character
				x += 6;					// ' ' will add +2 as well
			case ' ':
				x = putChar(x,y,flag,' ');
				wordWrap(&x, &y, right_edge, left_edge, text);
				break;
			default:
				x = putChar(x,y,flag,c);
		}
    }
}

int stringLength(const char *text) {
	int text_length = 0;
	int i = 0;
	while(text[i] != '\0') {
		if(text[i] == '\2')
			i++;
		i++;
		text_length++;
	}
	return text_length;
}

int intWidth(int num) {
	int width = 0;
	while(num > 0) {
		width += LETTER_WIDTH['0'+num%10-' '];
		num /= 10;
	}
	return width;
}


int stringWidth(const char *text) {
	int text_length = 0;
	int i=0;
	while(text[i] != '\0') {
		if(text[i] == '\2')
			i++;
		text_length+=LETTER_WIDTH[i];
		i++;
	}
	return text_length;
}


/*****************
 * prints integer
 * --------------
 * Returns text's new x value
 *****************/
int putInt(int x, int y, e_speed flag, int num) {
	int i = num/10;
	if(i>0)
		x = putInt(x,y,flag,i);
	return putChar(x,y,flag,num%10 + '0');
}

int putChar(int x, int y, e_speed flag, char c) {
	int spriteId = c-' ';
	u16 *vram = BG_MAP_RAM_SUB(0);
	u16 *sprite = (u16 *)fontTiles + (spriteId)*32;

	int i,j;
	for(i=0;i<8;i++) {
		for(j=0;j<4;j++) {
			int sprite_word = *sprite++;
			if(x%2 == 0)
				vram[(y+i)*128+x/2+j] |= sprite_word;
			else {
				vram[(y+i)*128+x/2+j] |= (sprite_word&0x00FF)<<8;
				vram[(y+i)*128+x/2+j+1] |= sprite_word>>8;
			}
		}
	}
	delay(flag);
	return x + LETTER_WIDTH[spriteId];		// advance x
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
	drawBoxType(x,y,w,h,0);
}

void drawBoxType(int x, int y, int w, int h, int type) {
	type *= BOX_TILES;		// if the box is highlighted, use the other tile set
	u16 *box_map = BG_MAP_RAM_SUB(24);
	int i,j;
	box_map += 32*y+x;		// map width is 32
	// draw top row
	*box_map = TL+type;
	for(i = 0; i < w-2; i++) {
		box_map++;
		*box_map = T+type;
	}
	box_map++;
	*box_map = TR+type;
	box_map += 33-w;		// move to next line
	// draw middle area
	for(j = 0; j < h-2; j++) {
		*box_map = L+type;
		for(i = 0; i < w-2; i++) {
			box_map++;
			*box_map = C+type;
		}
		box_map++;
		*box_map = R+type;
		box_map += 33-w;	// move to next line
	}
	// draw bottom row
	*box_map = BL+type;
	for(i = 0; i < w-2; i++) {
		box_map++;
		*box_map = B+type;
	}
	box_map++;
	*box_map = BR+type;	
}

#define ORIG_PALETTE_VAL 0x665D

void drawBoxes(Box boxes[], int numBoxes, int selected) {
	int x,y,w,h,type,i;
	for(i = 0; i < numBoxes; i++) {
		x = boxes[i].x;
		y = boxes[i].y;
		w = boxes[i].w;
		h = boxes[i].h;
		if(i == selected || selected == 255) {
			BG_PALETTE_SUB[3] = ORIG_PALETTE_VAL - (boxes[i].counter/20)*(1<<5);
			type = 1;
			if(boxes[i].counter%2 == 0) {
				boxes[i].counter += 2;
					if(boxes[i].counter == 180)
						boxes[i].counter--;
			} else {
				boxes[i].counter -= 2;
					if(boxes[i].counter < 0)
						boxes[i].counter++;
			}
			
		} else {
			type = 0;
			boxes[i].counter = 0;
		}
		drawBoxType(x,y,w,h,type);
	}

	
}

// draws a box with text inside it
void drawTextBox(int x, int y, int w, int h, e_speed flag, const char *text) {
    drawBox(x,y,w,h+1);
    putString(x*8,y*8,(w-1)*8,h,flag,text);
}

void delText(int x, int y, int w, int h) {
	x+=4;
	y+=4;
	u16 *vram = BG_MAP_RAM_SUB(0);
	int i,j;
	for(i=0;i<h;i++) {
		for(j=0;j<w/2;j++) {
			if(x%2 == 0)
				vram[(y+i)*128+x/2+j] = 0;
			else {
				vram[(y+i)*128+x/2+j] &= 0x00FF;
				vram[(y+i)*128+x/2+j+1] &= 0xFF00;
			}
		}
	}
}

void delTextBox(int x, int y, int w, int h) {
	delText(x*8,y*8,(w-1)*8,(h-1)*8);
    u16 *box_map = BG_MAP_RAM_SUB(24)+32*y+x;
    int i,j;

    // erase box by overwriting it with 0s
    for(j = 0; j < h; j++) {
        for(i = 0; i < w; i++) {
            *box_map = 0;
            box_map++;
        }
        box_map += 32-w;	// move to next line
    }
}

void clrSubScreen() {
	delTextBox(0,0,32,24);
}
