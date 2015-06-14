typedef struct {
	char *name;
	char *description;
	int cost;
}Item;

#define I_ITEM		0
#define I_ARMOR		1
#define I_WEAPON	2

int findItemPos(int id);
bool receiveItem(int itemType, int itemId, int amt);
int countEquip(int itemType, int itemId);
