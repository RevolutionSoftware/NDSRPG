// main menu when pressing Y
char *menu_Y	=	"\1Items\n"
					"\1Equipment\n"
					"\1Stats\n"
					"\1Options\n"
					"\1Save\n"
					"\1Exit";

// Equipment menu
char *menu_equip_BL	=	"Weapon:\n %s\n\nArmor:\n %s";

// Stats menu
char *menu_stats	=	"Character: %s\n"
						"HP: %d/%d\n"
						"ATK: %d\n"
						"DEF: %d\n"
						"AGI: %d\n\n"
						"Weapon: %s\n"
						"Armor: %s\n"
						"State: ";

// Options menu
char *menu_options	=	"What do you need options for? Our game is perfect as-is!";

// Save menu
char *menu_save		=	"There is no saving you! MwahAHahHAH!";

// Confirmation dialog
char *yes_or_no		=	"Are you sure?\n\1Yes \1No";

// Stores
char *store_buy_txt	= "How many?\n< %d >\n\nPrice:\n%d";
char *store_items_txt = "Money:\n%d\n\nYou have:\n%d";

#define I_BANANA	0,0
#define I_OATMEAL	0,1
#define I_TENT		0,2
#define I_FLOWER	0,3
// a_none doesn't exist
#define A_TSHIRT	1,1
#define A_CHAINMAIL	1,2
#define A_LEG_ARMOR	1,3
// w_none doesn't exist
#define W_DAGGER	2,1
#define W_SWORD		2,2
#define W_PISTOL	2,3
#define W_BAZOOKA	2,4

char store_1_hello[] = "What can I do for ya?";
char store_1_goodbye[] = "That's all you wanted?";
int store_1_items[]	= {	A_LEG_ARMOR,
						I_BANANA,
						I_OATMEAL,
						I_TENT,
						I_FLOWER};

Store store_list[] = {{store_1_hello,store_1_goodbye,store_1_items,5}};
