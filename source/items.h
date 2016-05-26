#ifndef ITEMS_H
#define ITEMS_H
#define IT_SINGLE	0
#define IT_ALL		1
#define IT_OTHER	2

typedef struct {
	char *name;
	char *description;
	int cost;
	int type;	//
}Item;

typedef enum {
    I_BANANA,
    I_OATMEAL,
    I_TENT,
    I_FLOWER
} i_item;

#define I_ITEM		0
#define I_ARMOR		1
#define I_WEAPON	2

int findItemPos(int id);
bool receiveItem(int itemType, int itemId, int amt);
int countEquip(int itemType, int itemId);
void useItem(int itemPos, int pId);
#endif
