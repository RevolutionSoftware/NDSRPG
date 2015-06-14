typedef struct {
	char *name;
	char *description;
	int cost;
}Item;

int findItemPos(int id);
bool receiveItem(int itemId, int amt);
