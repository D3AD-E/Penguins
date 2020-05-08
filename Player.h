#ifndef PLAYER_H
#define PLAYER_H

typedef struct Player Player;

struct Player {
	char playerPseudo[500];
	int playeriD;// PlayerNumL
	int	playerPoints;//FishAmountL
	int penguinAmount;
};
#endif // !SQUARE_H