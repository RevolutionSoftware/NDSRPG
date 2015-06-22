typedef enum {
    A_STAND,
    A_WALK,
    A_KICK_L,
    A_PUNCH_L,
    A_PUNCH_R,
    A_KICK_H
} b_actions;

typedef struct {
	int id;			// 0 = first character, 1 = 2nd char, 2 = 3rd character
	int state;		// kick low, kick high, punch left, punch right, standing, running
	int frame;
	int x;
	int y;
	int target;
	int attackList[15];
}BattlePlayer;

typedef struct {
	int id;
	int frame;
	int state;
	int x;
	int y;
	int hp;
}BattleEnemy;

typedef struct {
	BattleEnemy list[6];
	int numEnemies;
}Enemy;


BattlePlayer b_player[3];
Enemy enemy;

void startBattle();
int battleMenu(int pId);
void loadEnemies();
void drawPlayers();
void drawEnemies();

int inputAttack(int pId);
void drawAttackBar(int length);
void playerAttack(int pId);
