#include <stdio.h>
#include <string.h>
#include "designFunctions.h"
#include "auxiliaryFunctions.h"
#include "boardFunction.h"

int ROWS = 0;
int COLUMNS = 0;

int main(int argc, char* argv[]) {
	Player* playersArray = NULL;
	Tile** gameBoard = NULL;

	if (argc == 2 && strcmp(argv[1], "id") == 0) {
		printf("%s", read_computer_pseudo());
	}
	else {
		return play_game(argc, argv, gameBoard, playersArray);
	}
}