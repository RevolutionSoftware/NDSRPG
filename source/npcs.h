#define NPC_ENTRY 6			// size of each entry in npcs/npcs.h

typedef struct {
	int set;
	int pos;
	int ctr;
}Path;

typedef struct {
	int x;					// x and y coordinates
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
void moveNPC(int id);
