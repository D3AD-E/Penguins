#include "boardFunction.h"

extern  int ROWS;
extern  int COLUMNS;


Tile** generate_board(Tile **gameBoard, int numberOfPlayers)
{
	//Let's create the board with the squares
	gameBoard = (Tile**)realloc(gameBoard,sizeof(Tile*) * ROWS);
	for (size_t i = 0; i < ROWS; i++)
	{
		gameBoard[i] = (Tile*)malloc(sizeof(Tile) * COLUMNS);
	}

	//We now fill the board
	fill_board(gameBoard);

	filling_board_verification(gameBoard, numberOfPlayers);

	save_board(gameBoard, "board.txt");

	

	return gameBoard;
}

void fill_board(Tile **board)
{

	int numberFishesToPlace;

	srand((unsigned)time(NULL)); //initialisation for the random fonction

	for (short rows = 0; rows < ROWS; rows++)
	{
		for (short columns = 0; columns < COLUMNS; columns++)
		{
			numberFishesToPlace = (rand() % 3) + 1; //Like that we will have 1 2 or 3 fish
			board[rows][columns].idPlayer = 0;
			board[rows][columns].fishNumber = numberFishesToPlace;
			board[rows][columns].rowNumber = rows;
			board[rows][columns].columnNumber = columns;
		}
	}
}

int save_board(Tile **board, char* fileName)
{
	FILE* fileBoard;

	if ((fileBoard = fopen(fileName, "r+")) != NULL) {
		
		//We place the cursor at the beginning of the file
		fseek(fileBoard, 0, SEEK_SET);


		//First of all we write the size of the board
		fprintf(fileBoard, "%d %d\n", ROWS, COLUMNS);

		//Secondly we will replace the board data
		for (size_t i = 0; i < ROWS; i++)
		{
			for (size_t j = 0; j < COLUMNS-1; j++)
			{
				fprintf(fileBoard,"%d%d ", board[i][j].fishNumber, board[i][j].idPlayer);
			}
			fprintf(fileBoard, "%d%d\n", board[i][COLUMNS-1].fishNumber, board[i][COLUMNS-1].idPlayer);
		}
		
		fclose(fileBoard);
	}
	else {
		//something bad happened
		fprintf(stderr, "Oh dear something bad happened while trying to open the file");
		exit(2);
	}



	return 0;
}

void save_id_players_file(Player *userData, int numberPlayers)
{
	FILE* inputFile = NULL;

	//We will try to open the board file and we place the cursor at the end
	if ((inputFile = fopen("board.txt", "a")) !=  NULL) {

		for (size_t i = 0; i < numberPlayers; i++)
		{
			fprintf(inputFile, "%s %d 0\n", userData[i], i + 1);
		}

		//We close the file
		fclose(inputFile);
	}
	else {
		fprintf(stderr, "Error code retrieved while trying to open the file");
		exit(2);
	}
}

int count_player_penguins(int idPlayer)
{
	int numberPenguins = 0, line = 0;
	FILE* inputFile = NULL;
	char dataInput[500];

	//We will try to open the board file
	if ((inputFile = fopen("board.txt", "r")) != NULL) {

		while (fgets(dataInput, 500, inputFile) != NULL) {
			line++;

			if (line == 1) {
				continue;
			}
			else if (line == ROWS + 2) {
				break;
			}

			for (short i = 1; i <= COLUMNS * 3; i += 3) {
				/*dataInput[i] - 48 is an operation to obtain the numerical value of
				dataInput[i] because the integer that we obtain is the ASCII code of the
				character*/
				if ((dataInput[i] - 48) == idPlayer) { numberPenguins++; }
			}
		}

		
	}
	else {
		printf("Error retrieved while trying to open the file: error code\n");
		exit(2);
	}

	//We close the stream with the file.
	fclose(inputFile);
	return numberPenguins;
}

void read_players_id()
{
	FILE* inputFile = NULL;
	char dataInput[500];

	//We will try to open the board file
	if ((inputFile = fopen("board.txt", "r")) == 0) {

		//We will place the cursor so as to read only the id of the players
		for (size_t i = 0; i <= ROWS; i++)
		{
			fgets(dataInput, 500, inputFile);
		}

		while (fgets(dataInput, 500, inputFile) != NULL)
		{
			printf("%s\n\n", find_id_player(dataInput));

		}

		//We close the stream with the file.
		fclose(inputFile);
	}
	else {
		fprintf(stderr, "Error retrieved while trying to open the file\n");
		exit(2);
	}
}

void delete_board()
{
	remove("board.txt");
}

void register_special_user(int idComputer,char *pseudoComputer, char *boardName)
{
	FILE *boardFile;
	char character_detected;

	if ((boardFile = fopen(boardName,"a+")) != NULL) {

		//We verify the previous character
		fseek(boardFile, -1, SEEK_END);
		character_detected = fgetc(boardFile);
		
		if(isdigit(character_detected)){
			fseek(boardFile, 0, SEEK_END);
			fprintf(boardFile, "\n%s %d %d", pseudoComputer, idComputer, 0);
		}
		else {
			fseek(boardFile, 0, SEEK_END);
			fprintf(boardFile, "%s %d %d", pseudoComputer, idComputer, 0);
		}

		//As the file was opened in append mode, we just need to add our id paramater
		fclose(boardFile);
	}
	else {
		//Something wrong happened

		fprintf(stderr, "Something bad happened while trying to happen this file\n");
		exit(2);
		//we free the memory
	}
}

void read_board_data(Tile **board, char *fileName)
{
	FILE *boardFile;
	char *data;
	char lineData[500];
	int numberFish;
	int idPlayer;

	if ((boardFile = fopen(fileName, "r")) != NULL) {

		//We skip the first line
		fgets(lineData, 500, boardFile);

		
		//We fill the array
		for (size_t i = 0; i < ROWS; i++)
		{
			//We read the line number i + 1
			fgets(lineData, 500, boardFile);


			for (size_t j = 0; j < COLUMNS; j++)
			{
				
				if (j == 0) {

					data = strtok(lineData, " ");

					if (sscanf(data, "%1d%1d", &numberFish, &idPlayer) != 2) {
						fputs("Bad format for describing a tile's data has been detected! Quitting the program..\n",stderr);
						exit(2);
					}
					
					board[i][j].fishNumber = numberFish;
					board[i][j].idPlayer = idPlayer;

				}
				else {
					data = strtok(NULL, " ");

					if (sscanf(data, "%1d%1d", &numberFish, &idPlayer) != 2) {
						fputs("Bad format for describing a tile's data has been detected! Quitting the program..\n", stderr);
						exit(2);
					}

					board[i][j].fishNumber = numberFish;
					board[i][j].idPlayer = idPlayer;
				}
			}
			
		}
		//we close the file
		fclose(boardFile);
	}
	else {
		//We could not open the file
		printf("An error occured while trying to open the file.\n");
		exit(2);
	}
}

void penguin_removal(Tile ** board, int PlayerNumL) //REMOVE A PENGUIN IF IT IS NOT ABLE TO MOVE
{
	bool BadPenguin;
	for (int j = 0; j < ROWS; j++)
	{
		for (int i = 0; i < COLUMNS; i++)
		{
			BadPenguin = false;
			if (board[j][i].idPlayer == PlayerNumL)
			{
				if (j == 0 && i == 0)
				{
					if (board[j][i + 1].fishNumber == 0 && board[j + 1][i].fishNumber == 0)
						BadPenguin = true;
				}
				else if (j == ROWS-1 && i == COLUMNS-1)
				{
					if (board[j][i - 1].fishNumber == 0 && board[j - 1][i].fishNumber == 0)
						BadPenguin = true;
				}
				else if (j == ROWS-1 && i == 0)
				{
					if (board[j - 1][i].fishNumber == 0 && board[j][i + 1].fishNumber == 0)
						BadPenguin = true;
				}
				else if (j == 0 && i == COLUMNS-1)
				{
					if (board[j + 1][i].fishNumber == 0 && board[j][i - 1].fishNumber == 0)
						BadPenguin = true;
				}
				else if (j == 0)
				{
					if (board[j][i + 1].fishNumber == 0 && board[j + 1][i].fishNumber == 0 && board[j][i - 1].fishNumber == 0)
						BadPenguin = true;
				}
				else if (i == 0)
				{
					if (board[j][i + 1].fishNumber == 0 && board[j + 1][i].fishNumber == 0 && board[j - 1][i].fishNumber == 0)
						BadPenguin = true;
				}
				else if (j == ROWS-1)
				{
					if (board[j][i + 1].fishNumber == 0 && board[j - 1][i].fishNumber == 0 && board[j][i - 1].fishNumber == 0)
						BadPenguin = true;
				}
				else if (i == COLUMNS-1)
				{
					if (board[j][i - 1].fishNumber == 0 && board[j - 1][i].fishNumber == 0 && board[j + 1][i].fishNumber == 0)
						BadPenguin = true;
				}
				else
				{
					if (board[j][i + 1].fishNumber == 0 && board[j][i - 1].fishNumber == 0 &&
						board[j - 1][i].fishNumber == 0 && board[j + 1][i].fishNumber == 0) //Check surroundings
						BadPenguin = true;
				}
				if (BadPenguin)
				{
					board[j][i].idPlayer = 0;
					board[j][i].fishNumber = 0;
				}
			}
		}
	}
	return;
}

void change_board(Tile ** board, int missingNumberOfTiles) {
	Tile **tilesArray; //Array containing the tiles with 2 or 3 penguins on it
	int numberTiles = 0; //integer than count the number of tiles with 2 or 3 penguins on it
	int sizeArray;
	int indexSelected;


	tilesArray = (Tile*)calloc(0,sizeof(Tile));

	//We are looking for the specific tiles
	for (size_t i = 0; i < ROWS; i++)
	{
		for (size_t j = 0; j < COLUMNS; j++)
		{
			if (board[i][j].fishNumber != 1) {
				
				numberTiles++;
			
				tilesArray = (Tile*)realloc(tilesArray, numberTiles * sizeof(Tile));
				tilesArray[numberTiles - 1] = &board[i][j];
			}
		}
	}

	//We compute the size of the array
	sizeArray = _msize(tilesArray) / sizeof(Tile*);

	//Now we will modify some tiles so as to fit the number of tiles with one penguin required
	srand(time(NULL));

	while (missingNumberOfTiles > 0) {
		//We select an index
		indexSelected = rand() % sizeArray;
		//We verify that the pointer points to a Tile
		if ( tilesArray[indexSelected] != NULL) {
			tilesArray[indexSelected]->fishNumber = 1;
			tilesArray[indexSelected] = NULL; // We prevent to select this tile once again
			missingNumberOfTiles--;
		}
	}

	free(tilesArray);
	return;

}

void read_board_size(char *nameFile)
{
	FILE *boardFile;

	int rowsNumber;
	int columnsNumber;


	if ((boardFile = fopen(nameFile, "r")) != NULL) {
		//file opened

		//Now we will retrieve the size of the array


		//First of all, we place the pointor at the beginning of the file (Security parameter)
		rewind(boardFile);

		if (fscanf(boardFile, "%d %d", &rowsNumber, &columnsNumber) != 2) {
			//Something  bad happened with the board file
			printf("Our algorithm was unabled to verify the size of board.\nPlease verify that the first line of the text file is written like this: m n.\n");
			exit(2);
		}

		//We pass now the board size values (rows and columns) to the global variables
		ROWS = rowsNumber;
		COLUMNS = columnsNumber;

		fclose(boardFile);
	}
	else {
		printf("Oh dear something bad happened while trying to open the file..\n");
		exit(2);
	}
}

void update_board_movement(Tile ** board,const Position initialPosition, const Position finalPosition, int *numberFishCollected, int idComputer)
{
	int newValueFishCollected;

	//First of all we collect the fish
	newValueFishCollected = board[initialPosition.rowNumber][initialPosition.columnNumber].fishNumber;

	*numberFishCollected = *numberFishCollected + newValueFishCollected;


	board[finalPosition.rowNumber][finalPosition.columnNumber].idPlayer = idComputer;


	//We transform the former penguin's tile into water
	board[initialPosition.rowNumber][initialPosition.columnNumber].fishNumber = 0;
	board[initialPosition.rowNumber][initialPosition.columnNumber].idPlayer = 0;
}

int determine_computer_iD(char * boardFileName)
{
	FILE* boardFile;
	char data[500];
	char dataCopy[500];
	int iDLastPlayer;
	int idPlayersFirstPosition;


	if ((boardFile = fopen(boardFileName, "r")) != NULL) {
		//The file was opened

		//We fist need to verify if other players are registered on the file
		for (size_t i = 1; i <= ROWS+1; i++)
		{
			fgets(data, 500, boardFile);
		}

		//We verify if other players are registered on the board
		idPlayersFirstPosition = ftell(boardFile);
		if (fgets(data, 500, boardFile) == NULL) { 
			fclose(boardFile);
			return 1; 
		}
		//Otherwise we will take the id of the last computer registered incremented by one
		fseek(boardFile, idPlayersFirstPosition, SEEK_SET);
	

		while (fgets(data, 500, boardFile) != NULL) {

			if (sscanf(data, "%*s %d", &iDLastPlayer) != 1) {
				fprintf(stderr, "Impossible to read the id of the last player.. Quitting the program!\n");
				exit(2);
			}
		}

		
	}
	else {
		//Something bad happened
		fprintf(stderr, "Oh dear something bad happened while trying to open the file\n");
		exit(2);
	}

	fclose(boardFile);
	return iDLastPlayer + 1;
}

void update_points(char *boardFileName,int iDComputer, int newValue)
{
	FILE *boardFile;
	char data[500];
	char pseudoRead[500];
	int positionBeginningLine;
	int previousPlayerPoints;

	if ((boardFile = fopen(boardFileName, "r+")) != NULL) {
		//the file was opened successfully

		for (size_t i = 1; i <= ROWS+1; i++)
		{
			fgets(data, 500, boardFile);
		}


		//We are looking for the line on which our pseudo is written
		while (1) {

			positionBeginningLine = ftell(boardFile);

			fgets(data, 500, boardFile);

			sscanf(data, "%s", pseudoRead);


			if (strcmp(pseudoRead, read_computer_pseudo()) == 0) {
				
				//We replace the cursor at the beginning of the line
				fseek(boardFile, positionBeginningLine, SEEK_SET);

				//We retrieve the previous points of the player
				sscanf(data, "%*s %*d %d", &previousPlayerPoints);

				//We replace the current line by the line containing the update
				fprintf(boardFile, "%s %d %d", read_computer_pseudo(), iDComputer, newValue+previousPlayerPoints);

				break;


			}
		}

		fclose(boardFile);

	}
	else {
		//an error occured while trying to open the file
		fprintf(stderr, "Oh dear something went bad while trying to open the file\n");
		exit(2);
	}
}

int read_computer_id(char * boardFileName)
{
	FILE *boardFile;
	char data[500];
	char pseudoRead[500];
	int iDComputer;
	int beginningOfLine;


	if ((boardFile = fopen(boardFileName, "r")) != NULL) {
		//file opened

		//We are looking for the ids
		for (size_t i = 1; i <= ROWS+1; i++)
		{
			fgets(data, 500, boardFile);
		}

		while (1) {
			beginningOfLine = ftell(boardFile);

			fgets(data, 500, boardFile);

			sscanf(data, "%s", pseudoRead);

			if (strcmp(pseudoRead, read_computer_pseudo()) == 0) {
				sscanf(data, "%*s %d", &iDComputer);

				break;
			}
		}

		fclose(boardFile);
		return iDComputer;
	}
	else {
		//problem occured

		fprintf(stderr, "an error occured while trying to open the file.\n");
		exit(2);
	}
}



