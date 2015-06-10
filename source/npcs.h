typedef struct {
	int x;					// x and y coordinates
	int y;
	int string_id;			// text to display when spoken to
	int sprite_id;			// 
	int path;				// the path the NPC will take
	int direction;			// where sprite is facing
	int anim_frame;
	bool active;			// whether this NPC slot is being used or not
}NPC;

void loadNPCs(int map);
void animateNPCs(Drawable *player);

