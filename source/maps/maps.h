#include "map0.h"
#include "map1.h"

map_t map_list[2] = {{map0,map0_tiledata,map0_obj_data,80,64},
		{map1,map1_tiledata,map1_obj_data,32,32},
		};

u16 map_change_list[4][5] = {{1,4,4,11,8},
		{1,8,0,12,3},
		{1,10,13,15,18},
		{0,0,0,3,6},
		};