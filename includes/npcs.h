#ifndef NPCS_H
#define NPCS_H
#define NPC_ENTRY 10			// size of each entry in npcs/npcs.h

typedef struct {
	int set;
	int pos;
	int ctr;
	int action;
}Path;

typedef struct {
	int startx;				// starting x and y coordinates
	int starty;
	int x1;					// NPCs boundaries
	int y1;
	int x2;
	int y2;
	int x;					// current x and y coordinates
	int y;
	int string_id;			// text to display when spoken to
	int sprite_id;			//
	Path path;				// the path the NPC will take
	int direction;			// where sprite is facing
	int anim_frame;
	bool active;			// whether this NPC slot is being used or not
}NPC;

void loadNPCs(int map);
void animateNPCs();
bool NPCCollision(int id, int pdir, int ndir);
void moveNPCs();
#endif
