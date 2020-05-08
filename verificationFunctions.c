#include "verificationFunctions.h"

extern int ROWS;
extern int COLUMNS;


int numberVerification(char data[500])
{
	int dataNumber; //Numeric value of data
	//We first verify that the data retrieved is a number
	if (atoi(data) != 0) {
		dataNumber = atoi(data);
		
		//Now we verify that the number indicating is between 1 and 9
		if (dataNumber >= 2 && dataNumber <= 9) {
			return 1;
		}
		else {
			return 0;
		}
	}
	else {
		return 0;
	}
}

int pseudo_verification(char * pseudoToTest, Player* currentPlayersList, int numberPlayersCurrentlyRegistered)
{
	if (numberPlayersCurrentlyRegistered == 1) {
		return 1;
	}
	else {
		for (size_t i = 0; i < numberPlayersCurrentlyRegistered; i++)
		{
			if (strcmp(pseudoToTest, currentPlayersList[i].playerPseudo) == 0) {
				return 0;
			}
		}
		return 1;
	}
}

int board_verification()
{
	FILE *boardFile;
	//We try to open the file
	if ((boardFile = fopen("board.txt", "r") != NULL)) {
		//the file exists
		fclose(boardFile);
		return 1;
	}
	else {
		//the file does not exit
		return 0 ;
	}
}

void filling_board_verification(Tile **board, int numberOfPlayers)
{
	int numberOfTiles = 0; //number of tiles containing only 1 fish
	int numberOfTilesNeeded = 0; 

	/*We calculate the number of tiles with one fish that should 
	be present on the board*/
	switch (numberOfPlayers) {
	case 2:
		/*the value 5 corresponds to a security parameter because in some cases 
		we will have tiles with a penguin next to each other (making possible 
		movements impossible). So we try to give our players as many choices as 
		possible*/
		numberOfTilesNeeded = 2 * 4 + 5; 
		break;
	case 3:
		numberOfTilesNeeded = 3 * 3 + 5;
		break;
	
	case 4:
	case 5:
	case 6:
	case 7:
	case 8:
	case 9:
		numberOfTilesNeeded = numberOfPlayers * 2 + 5;
		break;
	}

	/*We need to verify that the number of tiles containing only one fish is greater or 
	equal to the number of penguins that players need to place */

	for (size_t i = 0; i < ROWS ; i++)
	{
		for (size_t j = 0; j < COLUMNS; j++)
		{
			if (board[i][j].fishNumber == 1) { 
				numberOfTiles += 1;
			}
		}
	}



	if (numberOfTiles >= numberOfTilesNeeded) {
		//there are a sufficient number of tiles containing only one fish
		return;
	}
	else {
		//We need to modify the board
		change_board(board, numberOfTilesNeeded - numberOfTiles);
		return;
	}
}

int check_movement_possible(Tile ** board,const Position penguinPosition)
{
	int numberPossibleMovement = 0;
	

	//Verification for the tile located on the left side of the current tile
	if((penguinPosition.columnNumber - 1) >= 0){
		//We verify whether there is fish on the selected tile and no other players
		if (!(board[penguinPosition.rowNumber][penguinPosition.columnNumber - 1].fishNumber == 0 ||
			board[penguinPosition.rowNumber][penguinPosition.columnNumber - 1].idPlayer != 0)) {

			numberPossibleMovement++;
		}
	}


	//Now we verify the tile located on the right side of the current tile
	if ((penguinPosition.columnNumber + 1) <= COLUMNS-1) {
		//We verify whether there is fish on the selected tile and no other players
		if (!(board[penguinPosition.rowNumber][penguinPosition.columnNumber + 1].fishNumber == 0 ||
			board[penguinPosition.rowNumber][penguinPosition.columnNumber + 1].idPlayer != 0)) {

			numberPossibleMovement++;
		}
	}


	//Now we verify the tile located on the upper side of the current tile
	if ((penguinPosition.rowNumber - 1) >= 0) {
		//We verify whether there is fish on the selected tile and no other players
		if (!(board[penguinPosition.rowNumber - 1][penguinPosition.columnNumber].fishNumber == 0 ||
			board[penguinPosition.rowNumber - 1][penguinPosition.columnNumber].idPlayer != 0)) {

			numberPossibleMovement++;
		}
	}


	//Finally we verify the tile located on the bottom side of the current tile
	if ((penguinPosition.rowNumber + 1) <= ROWS-1) {
		//We verify whether there is fish on the selected tile and no other players
		if (!(board[penguinPosition.rowNumber + 1][penguinPosition.columnNumber].fishNumber == 0 ||
			board[penguinPosition.rowNumber + 1][penguinPosition.columnNumber].idPlayer != 0)) {

			numberPossibleMovement++;
		}
		
	}

	return numberPossibleMovement > 0 ? 1 : 0;
}

short computer_registered_on_file(char * boardFileName)
{
	short computeriDFound = 0;
	FILE* boardFile;
	char data[500];
	char pseudoName[500];

	if ((boardFile = fopen(boardFileName, "r")) != NULL) {
		//The file was opened successfully

		//We need to skip the m + 1 lines so as to read only the id of the players
		
		for (size_t i = 1; i <= ROWS+1; i++)
		{
			fgets(data, 500, boardFile);
		}

		while (fgets(data,500,boardFile) != NULL) {
			sscanf(data, "%s", pseudoName);
			
			if (strcmp(pseudoName, read_computer_pseudo()) == 0) {
				computeriDFound = 1;
				break;
			}
		}

		fclose(boardFile);
	}
	else {
		//An error occured while trying to open the file
		fprintf(stderr, "Oh dear something bad happened while trying to open the file.");
		exit(2);
	}

	return computeriDFound;
}
