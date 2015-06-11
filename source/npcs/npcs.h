// 				   x, y,textid,spriteid,frame,path set
int map0_npc[] = {60,30,4,3,2,0,
				50,50,4,3,1,1,
				-1};

int map1_npc[] = {0,30,4,3,2,0,
				460,330,4,0,2,0,
				-1};

int *npc_list[] = {map0_npc,map1_npc};

#define WALK_LEFT(x)	0,x
#define WALK_RIGHT(x)	1,x
#define WALK_DOWN(y)	2,y
#define WALK_UP(y)		3,y
#define WAIT(frames)	4,frames
#define FACE(dir)		
#define PATH_END		-1

int npc_path0[]	= {WALK_RIGHT(40),WALK_DOWN(30),WALK_LEFT(40),WALK_UP(30),PATH_END};
int npc_path1[]	= {WALK_LEFT(50),WAIT(20),WALK_RIGHT(50),WAIT(40),PATH_END};
/*
 * move randomly
 * move to x position
 * start
 * 
 * 
 */ 

int *npc_paths[] = {npc_path0,npc_path1};
