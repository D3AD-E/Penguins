#include "AIfunctions.h"


extern int ROWS;
extern int COLUMNS;


int ai_placement(Tile **board, Player *currentPlayer)
{
	srand((unsigned)time(NULL)); // seed for random numbers generator
	
	/* those variables store number of fish in the column(row) with the
	largest amount of fish */
	short maxColumnPoints = 0, maxRowPoints = 0;
	
	short columnPoints = 0, rowPoints = 0;
	// indices of the column and row with the largest amount of fish
	size_t maxColumn, maxRow;
    
    // for loop checking columns
	for (size_t i = 0; i < COLUMNS; i++)
	{
		for (size_t j = 0; j < ROWS; j++)
		{
			// if there's fish, it'll add number of them into columnPoints
			columnPoints += board[j][i].fishNumber;
		}

		if (columnPoints > maxColumnPoints)
		{
			/* if column has more fish then previous column with max amount
			it becomes new maxColumn */
			maxColumnPoints = columnPoints;
			maxColumn = i;
		}
	}
    
    // for loop checking rows
	for (size_t i = 0; i < ROWS; i++)
	{
		for (size_t j = 0; j < COLUMNS; j++)
		{
			// if there's fish, it'll add number of them into rowPoints
			rowPoints += board[i][j].fishNumber;
		}

		if (rowPoints > maxRowPoints)
		{
			/* if row has more fish then previous column with max amount
			it becomes new maxRow */
			maxRowPoints = rowPoints;
			maxRow = i;
		}
	}

	size_t i;

	if (maxRowPoints > maxColumnPoints)
	{
		do // if the largest amount is in row
		{
			i = rand() % COLUMNS; // random index from chosen row
			
			// if tile is nonoccupied and contains only one fish
			if (board[maxRow][i].idPlayer == 0 && board[maxRow][i].fishNumber == 1)
			{
				// updating the board
				board[maxRow][i].fishNumber--;
				board[maxRow][i].idPlayer = currentPlayer->playeriD;

				// updating player's info
				currentPlayer->penguinAmount--;
				currentPlayer->playerPoints++;
			}

		} while (board[maxRow][i].idPlayer !=0 && board[maxRow][i].fishNumber != 1);
	}
	else
	{
		do // if the largest amount is in column
		{
			i = rand() % ROWS; // random index from chosen column
            
            // if tile is nonoccupied and contains only one fish
			if (board[i][maxColumn].idPlayer == 0 && board[i][maxColumn].fishNumber == 1)
			{
				// updating the board
				board[i][maxColumn].fishNumber--;
				board[i][maxColumn].idPlayer = currentPlayer->playeriD;
                
                // updating player's info
				currentPlayer->penguinAmount--;
				currentPlayer->playerPoints++;
			}

		} while (board[i][maxColumn].idPlayer != 0 && board[i][maxColumn].fishNumber != 1);
	}

	return 1; 
}


int ai_movement(Tile ** board, int idComputer, int *fishEarned)
{
	Position desiredTile;
	//Array that will store where the computer's penguins are placed on the board and they can move
	Position *penguinsPosition;
	Position *arrayPosition = NULL;
	//For each penguin it will save all the tiles containing an interesting number of fish (best 3)
	Position **specificTiles = NULL;

	int **tilePoints; //Used to select the best tile possible
	int numberPenguins = 0;
	int numberFishLookingFor = 3; //The most interesting
	int indexInitialPosition;

	/*Like a boolean saying that one of the penguin can make a movement that will permit 
	to gain a maximum of point*/
	int penguinWithPotential = 0;



	//We create the penguinsPositionArray
	penguinsPosition = (Position*) malloc(0 * sizeof(Position));
	
	
	//We need to go through the board so as to detect where our penguins are placed
	for (size_t i = 0; i < ROWS; i++)
	{
		for (size_t j = 0; j < COLUMNS; j++)
		{
			if (board[i][j].idPlayer == idComputer) {


				//We create a position object of the current tile
				Position currentTile = { i,j };

				//We verify if this penguin can move or not
				if (check_movement_possible(board, currentTile)) {
					//one of the penguin (which can move) is detected
					numberPenguins++;

					penguinsPosition = (Position*)realloc(penguinsPosition, numberPenguins * sizeof(Position));

					penguinsPosition[numberPenguins - 1].rowNumber = i;
					penguinsPosition[numberPenguins - 1].columnNumber = j;

				}
				else {
					//This penguins cannot move anymore we need to remove it from the board.
					*fishEarned = *fishEarned + board[i][j].fishNumber;
					board[i][j].fishNumber = 0;
					board[i][j].idPlayer = 0;
				}
			}
		
		}
	}

	

	//Now we have an eventual list of penguins which can move (this list can be empty)
	if (_msize(penguinsPosition) / sizeof(Position) == 0) { return 1; }
	
	//We can now evaluate the best movement

	/*First we create the array where we will save for each penguin the tiles on
	which they can move. Those tiles need to have an interesting number of fish  like 3*/

	specificTiles = (Position**)calloc(0, sizeof(Position));
	numberPenguins = 0; //Now it corresponds to the number of penguins with potential movement

	while (numberFishLookingFor != 0){

		changeColor(YELLOW);
		printf("We are looking for %d fish", numberFishLookingFor);
		changeColor(WHITE);

		
		for (int i = 0; i < _msize(penguinsPosition)/sizeof(Position); i++)
		{
			printf("\nPenguin %d\n\n",i+1);

			arrayPosition = retrieve_position_best_tiles(board, penguinsPosition[i], numberFishLookingFor);

			printf("\nNumber tiles detected: %d\n", _msize(arrayPosition) / sizeof(Position));

			if (_msize(arrayPosition) / sizeof(Position) > 0) {
				numberPenguins++;
				
				specificTiles = (Position**)realloc(specificTiles, numberPenguins * sizeof(Position));

				specificTiles[numberPenguins - 1] = arrayPosition;

				//We have found a potential penguin
				penguinWithPotential = 1;
			}
			else if (_msize(arrayPosition) / sizeof(Position) == 0 && penguinWithPotential == 1) {
				
				penguinsPosition = remove_element(penguinsPosition, i);
				i--;
			}
			
		}

		//If with have one penguin with potential we can quit
		if (penguinWithPotential == 1) {
			break;
		}
		else {
			numberFishLookingFor--;
		}	
	}

	
	changeColor(RED);
	printf("\nRemaining number of penguins: %d\n", _msize(specificTiles) / sizeof(Position*));
	changeColor(WHITE);

	//Now for each remaining penguins, we will attribute some points to their position

	//First we create the array for saving the points
	tilePoints = (int**)calloc(_msize(specificTiles) / sizeof(Position*), sizeof(int*));

	for (size_t i = 0; i < _msize(specificTiles)/sizeof(Position*); i++)
	{
		tilePoints[i] = (int*)calloc(_msize(specificTiles[i]) / sizeof(Position), sizeof(int));
	}

	
	//Now we fill the array
	for (size_t i = 0; i < _msize(tilePoints)/sizeof(int*); i++)
	{
		changeColor(RED);
		printf("\nPenguin %d\n", i+1);
		changeColor(WHITE);

		for (size_t j = 0; j < _msize(tilePoints[i])/sizeof(int); j++)
		{
			changeColor(YELLOW);
			tilePoints[i][j] = compute_tile_points(board, specificTiles[i][j]);
			printf("Tile: rowNumber: %d ~ columnNumber: %d | points: %d\n", 
				specificTiles[i][j].rowNumber, specificTiles[i][j].columnNumber, tilePoints[i][j]);
			changeColor(WHITE);
		}
	}

	

	//We now determine the best tile
	desiredTile = determine_best_tile(specificTiles, tilePoints, &indexInitialPosition);

	
	changeColor(YELLOW);
	printf("\nThe penguin will move to the tile: %d %d\n", desiredTile.rowNumber, desiredTile.columnNumber);
	changeColor(WHITE);
	
	//And now we just have to update the board
	update_board_movement(board, penguinsPosition[indexInitialPosition], desiredTile, fishEarned, idComputer);


	//We free the memory allocated for the penguinsPosition array
	free(penguinsPosition);

	//We free the memory allocated for the tilePoints array
	for (size_t i = 0; i < _msize(tilePoints)/sizeof(int*); i++)
	{
		free(tilePoints[i]);
	}
	free(tilePoints);

	//We free the memory allocated for the specificTiles array
	for (size_t i = 0; i < _msize(specificTiles)/sizeof(Position*); i++)
	{
		free(specificTiles[i]);
	}
	free(specificTiles);

	return 0;
}


Position * retrieve_position_best_tiles(const Tile ** board, const Position currentTilePos, const int numberFish)
{
	Position* tilesArray;
	Position tilePos;
	int numberMatchingTiles = 0;
	int index = 0;
	//We create the array
	tilesArray = (Position*)calloc(0, sizeof(Position));


	//First of all we are looking for the tiles locating on the upper part of the current tile
	changeColor(YELLOW);
	printf("Upper part\n");
	changeColor(WHITE);
	while (1) {
		index++;

		//First of all we need to verify that the new Position is not outside the board
		if (currentTilePos.rowNumber - index >= 0) {

			printf("Operation number: %d\n", index);

			//Now we verify that the tile is not an empty tile 0 0 or water if you prefer
			if (board[currentTilePos.rowNumber - index][currentTilePos.columnNumber].fishNumber == 0) {
				printf("Empty tile\n");
				break;
			}
			//Now we verify that this tile is not occupied by an other player
			else if (board[currentTilePos.rowNumber - index][currentTilePos.columnNumber].idPlayer > 0) {
				printf("Other player already present\n");
				break;
			}
			//Now we need to verify that there is the correct number of fish required on the tile
			else if (board[currentTilePos.rowNumber - index][currentTilePos.columnNumber].fishNumber == numberFish) {
				printf("Adding a new fish\n");
				//We detected a new position
				numberMatchingTiles++;

				//We allocate new memory
				tilesArray = (Position*)realloc(tilesArray, numberMatchingTiles * sizeof(Position));

				//We add the new position
				tilePos.rowNumber = currentTilePos.rowNumber - index;
				tilePos.columnNumber = currentTilePos.columnNumber;

				tilesArray[numberMatchingTiles - 1] = tilePos;
			}
			else {
				printf("The number of fish doesn't match the required value\n");
			}
		}
		else {
			break;
		}
	}

	//Secondly we will verify the tiles located on the bottom side of the current tile
	changeColor(YELLOW);
	printf("\nBottom part\n");
	changeColor(WHITE);

	//We reinitialize the index's value to 0
	index = 0;

	while (1) {
		index++;

		//First of all we need to verify that the new Position is not outside the board
		if (currentTilePos.rowNumber + index <= ROWS - 1) {

			printf("Operation number: %d\n", index);

			//Now we verify that the tile is not an empty tile 0 0 or water if you prefer
			if (board[currentTilePos.rowNumber + index][currentTilePos.columnNumber].fishNumber == 0) {
				printf("Empty tile\n");
				break;
			}
			//Now we verify that this tile is not occupied by an other player
			else if (board[currentTilePos.rowNumber + index][currentTilePos.columnNumber].idPlayer > 0) {
				printf("Other player already present\n");
				break;
			}
			//Now we need to verify that there is the correct number of fish required on the tile
			else if (board[currentTilePos.rowNumber + index][currentTilePos.columnNumber].fishNumber == numberFish) {
				printf("Adding a new fish\n");
				//We detected a new position
				numberMatchingTiles++;

				//We allocate new memory
				tilesArray = (Position*)realloc(tilesArray, numberMatchingTiles * sizeof(Position));

				//We add the new position
				tilePos.rowNumber = currentTilePos.rowNumber + index;
				tilePos.columnNumber = currentTilePos.columnNumber;

				tilesArray[numberMatchingTiles - 1] = tilePos;
			}
			else {
				printf("The number of fish doesn't match the required value\n");
			}
		}
		else {
			break;
		}
	}


	//Then we will look for the tiles located on the right side of the current tile
	changeColor(YELLOW);
	printf("\nRight part\n");
	changeColor(WHITE);

	//We reinitialise the index's value
	index = 0;

	while (1) {
		index++;

		//First of all we need to verify that the new Position is not outside the board
		if (currentTilePos.columnNumber + index < COLUMNS) {

			printf("Operation number: %d\n", index);

			//Now we verify that the tile is not an empty tile 0 0 or water if you prefer
			if (board[currentTilePos.rowNumber][currentTilePos.columnNumber+index].fishNumber == 0) {
				printf("Empty tile\n");
				break;
			}
			//Now we verify that this tile is not occupied by an other player
			else if (board[currentTilePos.rowNumber][currentTilePos.columnNumber+index].idPlayer > 0) {
				printf("Other player already present\n");
				break;
			}
			//Now we need to verify that there is the correct number of fish required on the tile
			else if (board[currentTilePos.rowNumber][currentTilePos.columnNumber+index].fishNumber == numberFish) {
				printf("Adding a new fish\n");
				//We detected a new position
				numberMatchingTiles++;

				//We allocate new memory
				tilesArray = (Position*)realloc(tilesArray, numberMatchingTiles * sizeof(Position));

				//We add the new position
				tilePos.rowNumber = currentTilePos.rowNumber;
				tilePos.columnNumber = currentTilePos.columnNumber + index;

				tilesArray[numberMatchingTiles - 1] = tilePos;
			}
			else {
				printf("The number of fish doesn't match the required value\n");
			}
		}
		else {
			break;
		}
	}
	

	//Finally we will look for the tiles located on the left side of the current tile
	changeColor(YELLOW);
	printf("\nLeft part\n");
	changeColor(WHITE);

	//We reinitialise the index's value
	index = 0;

	while (1) {
		index++;

		//First of all we need to verify that the new Position is not outside the board
		if (currentTilePos.columnNumber - index >= 0) {

			printf("Operation number: %d\n", index);

			//Now we verify that the tile is not an empty tile 0 0 or water if you prefer
			if (board[currentTilePos.rowNumber][currentTilePos.columnNumber - index].fishNumber == 0) {
				printf("Empty tile\n");
				break;
			}
			//Now we verify that this tile is not occupied by an other player
			else if (board[currentTilePos.rowNumber][currentTilePos.columnNumber - index].idPlayer > 0) {
				printf("Other player already present\n");
				break;
			}
			//Now we need to verify that there is the correct number of fish required on the tile
			else if (board[currentTilePos.rowNumber][currentTilePos.columnNumber - index].fishNumber == numberFish) {
				printf("Adding a new fish\n");
				//We detected a new position
				numberMatchingTiles++;

				//We allocate new memory
				tilesArray = (Position*)realloc(tilesArray, numberMatchingTiles * sizeof(Position));

				//We add the new position
				tilePos.rowNumber = currentTilePos.rowNumber;
				tilePos.columnNumber = currentTilePos.columnNumber - index;

				tilesArray[numberMatchingTiles - 1] = tilePos;
			}
			else {
				printf("The number of fish doesn't match the required value\n");
			}
		}
		else {
			break;
		}
	}

	return tilesArray;
}


int compute_tile_points(const Tile ** board, Position tilePosition)
{
	int points = 0;
	int index = 1;
	int movementIssueDetectedUpper = 0;
	int movementIssueDetectedBottom = 0;
	int movementIssueDetectedRight = 0;
	int movementIssueDetectedLeft = 0;

	
	while (index <= 3) {

		

		//UPPER PART
		if (tilePosition.rowNumber - index >= 0  && movementIssueDetectedUpper == 0) {

			if (board[tilePosition.rowNumber - index][tilePosition.columnNumber].idPlayer == 0 &&
				board[tilePosition.rowNumber - index][tilePosition.columnNumber].fishNumber != 0) {

				points += board[tilePosition.rowNumber - index][tilePosition.columnNumber].fishNumber * ((3 - index) + 1);
			}
			else {
				movementIssueDetectedUpper = 1;
			}

		}
		
		//BOTTOM PART
		if (tilePosition.rowNumber + index < ROWS && movementIssueDetectedBottom == 0) {

			if (board[tilePosition.rowNumber + index][tilePosition.columnNumber].idPlayer == 0 &&
				board[tilePosition.rowNumber + index][tilePosition.columnNumber].fishNumber != 0) {

				points += board[tilePosition.rowNumber + index][tilePosition.columnNumber].fishNumber * ((3 - index) + 1);
			}
			else {
				movementIssueDetectedBottom = 1;
			}

		}


		//RIGHT PART
		if (tilePosition.columnNumber + index < COLUMNS && movementIssueDetectedRight == 0) {

			if (board[tilePosition.rowNumber][tilePosition.columnNumber+index].idPlayer == 0 &&
				board[tilePosition.rowNumber][tilePosition.columnNumber+index].fishNumber != 0) {

				points += board[tilePosition.rowNumber][tilePosition.columnNumber+index].fishNumber * ((3 - index) + 1);
			}
			else {
				movementIssueDetectedRight = 1;
			}

		}

		//LEFT PART
		if (tilePosition.columnNumber - index >= 0 && movementIssueDetectedLeft == 0) {


			if (board[tilePosition.rowNumber][tilePosition.columnNumber-index].idPlayer == 0 &&
				board[tilePosition.rowNumber][tilePosition.columnNumber-index].fishNumber != 0) {

				points += board[tilePosition.rowNumber][tilePosition.columnNumber-index].fishNumber * ((3 - index) + 1);
			}
			else {
				movementIssueDetectedLeft = 1;
			}

		}

		index++;
	}

	return points;
}

Position determine_best_tile(const Position ** specificTiles, const int ** tilePoint, int *indexInitialPosition)
{
	int maxPoint = 0;
	int rowMaxPoint = 0;
	int columnMaxPoint = 0;

	Position bestTile;

	for (size_t i = 0; i < _msize(tilePoint)/sizeof(int*); i++)
	{
		for (size_t j = 0; j < _msize(tilePoint[i])/sizeof(int); j++)
		{
			if (tilePoint[i][j] > maxPoint) {
				maxPoint = tilePoint[i][j];
				rowMaxPoint = i;
				columnMaxPoint = j;

				//The value of i correspond to the value of the index of the initial position
				*indexInitialPosition = i;
			}
		}
	}

	bestTile.rowNumber = specificTiles[rowMaxPoint][columnMaxPoint].rowNumber;
	bestTile.columnNumber = specificTiles[rowMaxPoint][columnMaxPoint].columnNumber;

	return bestTile;
}
