
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
	int action;		// kick low, kick high, punch left, punch right, standing, running
	int frame;
	int x;
	int y;
}BattlePlayer;

BattlePlayer b_player[3];


void startBattle();
void drawPlayers();
