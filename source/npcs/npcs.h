// x, y, range x1, range y1, range x2, range y2, textid,spriteid,frame,path set
int map0_npc[] = {60,30,55,20,125,105,4,3,2,0,
				48,48,00,04,320,320,5,3,1,1,
				-1};

int map1_npc[] = {0,30,0,30,50,60,4,3,2,1,
				460,330,0,0,500,500,4,0,2,1,
				-1};

int *npc_list[] = {map0_npc,map1_npc};

#define WALK_RIGHT(x)	0,x
#define WALK_LEFT(x)	1,x
#define WALK_DOWN(y)	2,y
#define WALK_UP(y)		3,y
#define WAIT(frames)	4,frames
#define FACE(dir)		5,dir
#define RAND(frames)	6,frames
#define PATH_END		-1

int npc_path0[]	= {FACE(W_RIGHT),WAIT(30),WALK_RIGHT(4),WALK_DOWN(68),WALK_RIGHT(16),WAIT(10),FACE(W_UP),WAIT(150),WALK_RIGHT(40),WALK_UP(68),WALK_LEFT(60),FACE(W_DOWN),WAIT(50),PATH_END};
int npc_path1[]	= {RAND(16),PATH_END};

/*
 * move randomly
 * move to x position
 * start
 * 
 * 
 */ 

int *npc_paths[] = {npc_path0,npc_path1};
