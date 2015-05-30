#include "map.h"
#include "map1.h"

// tilemap array, tile data array, interaction array, width, height
map_t map_list[2] = {{map0,map0_tiledata,map0_obj_data,32,32},
					{map1,map1_tiledata,map1_obj_data,64,64}};

// map number, mapx, mapy, playerx, playery
u16 map_change_list[2][5] = {{1,16,20,30,30},
						{0,10,5,16,10}};
