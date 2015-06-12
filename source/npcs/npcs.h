// x, y, x range, y range, textid,spriteid,frame,path set
int map0_npc[] = {60,30,10,20,4,3,2,0,
				50,50,20,10,5,3,1,1,
				-1};

int map1_npc[] = {0,30,5,5,4,3,2,0,
				460,330,2,2,4,0,2,0,
				-1};

int *npc_list[] = {map0_npc,map1_npc};

#define WALK_RIGHT(x)	0,x
#define WALK_LEFT(x)	1,x
#define WALK_DOWN(y)	2,y
#define WALK_UP(y)		3,y
#define WAIT(frames)	4,frames
#define FACE(dir)		5,dir
#define PATH_END		-1

int npc_path0[]	= {FACE(W_RIGHT),WAIT(30),WALK_RIGHT(4),WALK_DOWN(68),WALK_RIGHT(16),WAIT(10),FACE(W_UP),WAIT(150),WALK_RIGHT(40),WALK_UP(68),WALK_LEFT(60),FACE(W_DOWN),WAIT(50),PATH_END};
int npc_path1[]	= {WALK_LEFT(50),WAIT(20),FACE(W_DOWN),WAIT(200),WALK_RIGHT(50),WAIT(20),FACE(W_UP),WAIT(20),PATH_END};

/*
 * move randomly
 * move to x position
 * start
 * 
 * 
 */ 

int *npc_paths[] = {npc_path0,npc_path1};
