#include "auxiliaryFunctions.h"

extern const int ROWS;
extern const int COLUMNS;

jmp_buf ebuf;


int play_game(int argc, char* argv[], Tile **gameBoard, Player *playersArray)
{
	
	int numberPengueesToPlace;
	int placementDone = 0; //Boolean to check if all the penguees have been placed
	int noMoreMovement = 0; //Boolean to check if all the penguees have been placed
	GamePhase gamePhase;
	Player computer;
	setjmp(ebuf);
	system("cls");

	//We need to detect if we are playing in automatic or interactive mode 
	if (argc == 1) {
		//Interactive mode
		main_menu(gameBoard, playersArray);
		return 0;
	}
	else {
		//Automatic mode

		if (argc == 4) {
			//Movement command line parameters

			//We read the size of the board before filling it
			read_board_size(argv[2]);

			

			//We allocate memory so as to create the board
			gameBoard = (Tile**)realloc(gameBoard, ROWS * sizeof(Tile*));
			for (size_t i = 0; i < ROWS; i++)
			{
				gameBoard[i] = (Tile*)calloc(COLUMNS, sizeof(Tile));
			}

			//We now need to read the board's data
			read_board_data(gameBoard, argv[2]);

		}
		else {
			//Placement command line paramaters
			
			read_board_size(argv[3]);

			//We allocate memory so as to create the board
			gameBoard = (Tile**)realloc(gameBoard, ROWS * sizeof(Tile*));

			for (size_t i = 0; i < ROWS; i++)
			{
				gameBoard[i] = (Tile*)calloc(COLUMNS, sizeof(Tile));
			}

			//We now need to read the board's data
			read_board_data(gameBoard, argv[3]);

			//For placement phase we need to verify that our computer is registered
			if (!computer_registered_on_file(argv[3])) {

				//And we register our computer on the file
				register_special_user(determine_computer_iD(argv[3]), read_computer_pseudo(), argv[3]);
			}

		}

		

		//Then, we create a new player that it will play by the computer

		computer.playeriD = read_computer_id(argc == 4 ? argv[2] : argv[3]); // we need to change this id
		strcpy(computer.playerPseudo, read_computer_pseudo());
		computer.playerPoints = 0; // we need to use the function to collect the points
		computer.penguinAmount = count_player_penguins(computer.playeriD);


		
		

		



		/*We will now detect each phase of the game, our program should execute*/
		if (detect_game_phase(argv[1])) {

			/*###########################################
							 PLACEMENT PHASE
			#############################################*/
			gamePhase = PLACEMENT;

			

			/*We verify that this special player (computer) is registered in the file
			by verifying the number of its penguins currently placed on the board*/



			//We first calculate the max number of penguins to place on the board
			numberPengueesToPlace = retrieve_max_penguins(argv[2]);


			//Then we try the placement phase
			if (penguin_placement(numberPengueesToPlace,&computer,gameBoard,AUTOMATIC)) {
				placementDone = 1;
			}
		}
		else {

			/*###########################################
							MOVEMENT PHASE
			#############################################*/
			gamePhase = MOVEMENT;
			//We specify that we are in movement phase

			
			//We verify that we have penguins otherwise we can skip the movement procedure
			if (count_player_penguins(computer.playeriD) == 0) {
				noMoreMovement = 1;
				goto EXITPROCEDURE;
			}

			
			noMoreMovement = penguin_move(gameBoard, AUTOMATIC, &computer, 1);


			//You can now save the changement on the board file

			save_board(gameBoard, argv[2]);

			update_points(argv[2], computer.playeriD, computer.playerPoints);

		}

	EXITPROCEDURE:


		//before quitting the program, we free the memory allocated for the board.
		//so as to avoid memory's leak...
		for (size_t i = 0; i < ROWS; i++)
		{
			free(gameBoard[i]);
		}
		free(gameBoard);



		//We return a specific value 1 or 0 depending on the phase we are
		switch (gamePhase) {

		case PLACEMENT:
			return placementDone == 1 ? 1 : 0;

		case MOVEMENT:
			return noMoreMovement == 1 ? 1 : 0;

		default:
			return 0;
		}
	}
}


int detect_game_phase(char *phase)
{
	char *dataInput = NULL;
	char delimiter[2] = "=";

	dataInput = strtok(phase, delimiter);
	dataInput = strtok(NULL, delimiter);

	return strcmp(dataInput, "movement") == 0 ? 0 : 1;
}




int getDataInput(char data[500])
{
	char* characterPosition;

	if (fgets(data, 500, stdin) != NULL) {
		characterPosition = strchr(data, '\n');

		if (characterPosition != NULL) {
			*characterPosition = '\0';
		}
		return 0;
	}
	else {
		//A problem occured, we return the code error 1
		return 1;
	}
}

#ifdef _WIN64
void setCursorPosition(int X, int Y)
{
	COORD newCoordinates = { X,Y };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), newCoordinates);
}

COORD getCursorPosition()
{
	COORD currentPosition;
	CONSOLE_SCREEN_BUFFER_INFO coinfo; //element that will contain the position of the cursor

	//We retrieve information about the console
	GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &coinfo);

	currentPosition.X = coinfo.dwCursorPosition.X;
	currentPosition.Y = coinfo.dwCursorPosition.Y;

	return currentPosition;
}

void scrollToTop()
{
	CONSOLE_SCREEN_BUFFER_INFO coinfo; //element that will contain the current position of the cursor
	SMALL_RECT rectangle; //A rectangle that will have the dimension of the windows console

	GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &coinfo); 


	//Then we define the new coordinate of our rectangle
	rectangle.Top =- (SHORT)coinfo.dwCursorPosition.Y;
	rectangle.Bottom = -(SHORT)coinfo.dwCursorPosition.Y;
	rectangle.Left = 0;
	rectangle.Right = 0;

	//We now move our rectangle to its new coordinate
	SetConsoleWindowInfo(GetStdHandle(STD_OUTPUT_HANDLE),FALSE, &rectangle);
}

Position get_coordinate_desired_tile(COORD topBoardPosition, COORD bottomBoardPosition, COORD* desiredTilePosition) {
	Position desiredTile = { 0,0 };
	COORD originalPosition = getCursorPosition();
	int columnNumber = 0;
	int rowNumber = 0;
	char keyTapped;

	//We set the initial position
	
	originalPosition.X = topBoardPosition.X + 2*2;
	originalPosition.Y = topBoardPosition.Y + 1;
	
	if (desiredTilePosition == NULL) {
		setCursorPosition(originalPosition.X, originalPosition.Y);
	}
	else {
		rowNumber = desiredTilePosition->Y;
		columnNumber = desiredTilePosition->X;
		setCursorPosition(originalPosition.X + (4 * columnNumber),
			originalPosition.Y + (2 * rowNumber));
	}

	while (1) {
		while (!_kbhit())
		{
			//We are waiting for the user to tap on a valid key
		}


		keyTapped = _getch();
		keyTapped = keyTapped != -32 ? keyTapped : _getch(); //We do that two times if the key detected is an arrow key

		switch (keyTapped) {

		case 27:
			longjmp(ebuf, 0);
			break;

		case 72: //arrow up
			if (rowNumber - 1 >= 0) {
				rowNumber--;
				setCursorPosition(originalPosition.X + (4 * columnNumber),
					originalPosition.Y + (2 * rowNumber));
			}
			break;

		case 80: //arrow down
			if (rowNumber + 1 < ROWS) {
				rowNumber++;
				setCursorPosition(originalPosition.X + (4 * columnNumber),
					originalPosition.Y + (2 * rowNumber));
			}
			break;

		case 77: //arrow right
			if (columnNumber + 1 < COLUMNS) {
				columnNumber++;
				setCursorPosition(originalPosition.X + (4 * columnNumber),
					originalPosition.Y + (2 * rowNumber));
			}
			break;

		case 75: //arrow left
			if (columnNumber - 1 >= 0) {
				columnNumber--;
				setCursorPosition(originalPosition.X + (4 * columnNumber),
					originalPosition.Y + (2 * rowNumber));
			}
			break;

		case 13:
			goto EXIT;
			break;

		default:
			break;
		}
	}
EXIT:

	//We set the cursor at the bottom of the board 
	setCursorPosition(bottomBoardPosition.X, bottomBoardPosition.Y);

	//We send the coordinates of the desired tile
	desiredTile.rowNumber = rowNumber;
	desiredTile.columnNumber = columnNumber;

	return desiredTile;
}
#endif


char * find_id_player(char * data)
{
	return strtok(data," ");
}



int penguin_placement(int maxPenguins, Player *currentPlayer, Tile ** board, ModeGame game_mode)
{
	#ifdef _WIN64
	COORD topBoardPosition;
	COORD bottomBoardPosition;
	COORD messageErrorPosition;
	#endif


	Position desiredTile;
	char* messageError;

	
	//We first clean the console
	system("cls");
	
	
	if (game_mode == INTERACTIVE) {
		#ifdef _WIN64
		//First of all we verify that the player can place a penguee
		if (count_player_penguins(currentPlayer->playeriD) != maxPenguins) {

			while (1) {
				print_placement_phase();

				//We retrieve the position of the top of the board
				topBoardPosition = getCursorPosition();

				//We display the board
				print_board(board);


				//We retrieve the position of the bottom of the board
				bottomBoardPosition = getCursorPosition();

				//We highlight in yellow the penguins of the player
				color_player_penguins(board, currentPlayer->playeriD, topBoardPosition, bottomBoardPosition);

				/*We enter in the verification step. As a remainder, the player should place his/her
				penguin on a tile containing one fish and with no player on it*/

				setCursorPosition(bottomBoardPosition.X, bottomBoardPosition.Y + 2);
				printf("Please select a tile (%s)\n", currentPlayer->playerPseudo);

				//We also print the number of remaining fish to place
				changeColor(RED);
				printf("Remember that you still have to place %d penguins on the board\n",maxPenguins - count_player_penguins(currentPlayer->playeriD));
				changeColor(WHITE);

				//The player selects a tile
				desiredTile = get_coordinate_desired_tile(topBoardPosition, bottomBoardPosition,NULL);

				//We verify the tile
				if (board[desiredTile.rowNumber][desiredTile.columnNumber].idPlayer == 0 &&
					board[desiredTile.rowNumber][desiredTile.columnNumber].fishNumber == 1) {
					//We can place the user
					board[desiredTile.rowNumber][desiredTile.columnNumber].idPlayer = currentPlayer->playeriD;

					//We will remove the fish from the tile and give one point to the player
					board[desiredTile.rowNumber][desiredTile.columnNumber].fishNumber = 0;
					
					currentPlayer->playerPoints = currentPlayer->playerPoints + 1;


					changeColor(YELLOW);
					setCursorPosition(bottomBoardPosition.X, bottomBoardPosition.Y + 4);
					printf("Penguin placed.");
					save_board(board, "board.txt");
					changeColor(WHITE);



					Sleep(1000);
					break;
				}
				else {

					changeColor(RED);

					setCursorPosition(bottomBoardPosition.X, bottomBoardPosition.Y + 4);

					if (board[desiredTile.rowNumber][desiredTile.columnNumber].idPlayer == 0 &&
						board[desiredTile.rowNumber][desiredTile.columnNumber].fishNumber == 0) {

						messageErrorPosition = getCursorPosition();
						messageError = "You cannot place your penguin in water...\n";
						printf("%s\n", messageError);
						
					}
					else if (board[desiredTile.rowNumber][desiredTile.columnNumber].idPlayer != 0) {

						messageErrorPosition = getCursorPosition();
						messageError = "An other player is already on this tile...\n";
						printf("%s\n", messageError);
					}
					else {
						messageErrorPosition = getCursorPosition();
						messageError = "You have to place your penguin on a tile containing only one fish...\n";
						printf("%s\n", messageError);
					}

					changeColor(WHITE);
					Sleep(1500);
					suppress_line_buffer(messageErrorPosition, strlen(messageError));
					system("cls");

				}
			}
			return 0;
		}
		else {
			return 1;
		}
		#endif
	}
	else {
		ai_placement(board, currentPlayer);
		return 0;
	}
}

void run_placement_stage(Tile ** board, Player *playersArray,int numberOfPlayers)
{
	int exitStage = 0;
	int numberOfPenguinsToPlace;

	//We read data from the board file


	//We define how many penguins the players need to place
	switch (numberOfPlayers)
	{
	case 2:
		numberOfPenguinsToPlace = 4;
		break;

	case 3:
		numberOfPenguinsToPlace = 3;
		break;

	case 4:
	case 5:
	case 6:
	case 7:
	case 8:
	case 9:
		numberOfPenguinsToPlace = 2;
		break;
	}

	while (!exitStage) {
		for (size_t i = 0; i < numberOfPlayers; i++)
		{
			exitStage = penguin_placement(numberOfPenguinsToPlace,&playersArray[i],board,INTERACTIVE);
		}
	}
	

	//We verify if there are penguins that are stuck
	for (size_t i = 1; i <= numberOfPlayers; i++)
	{
		penguin_removal(board, i);
	}

	return;
}

int retrieve_max_penguins(char * data)
{
	char *pointerToNumPenguins;

	pointerToNumPenguins = strchr(data, '=');

	pointerToNumPenguins++;

	return (*pointerToNumPenguins) - 48;
}

Player* register_players(Player *playersArray, int *numberPlayers)
{
	char userInput[500];
	int numberOfPlayers;
	

	changeColor(RED);
	printf("Please indicate the number of persons playing the game:\n");

	while (1) {
		changeColor(YELLOW);
		getDataInput(userInput);

		if (numberVerification(userInput)) {
			//The user entered a correct value we can quit the loop
			break;
		}
		else {
			changeColor(RED);
			printf("\nYour input is invalid. You entered either a non numerical value or a value which is out of the range 2...9\n");
		}

		changeColor(RED);
		printf("\nPlease indicate the number of persons playing the game:\n");
	}


	//We retrieve the number of players who will play the game
	numberOfPlayers = atoi(userInput);

	//Now we will create the array that will contain all the data concerning the players
	playersArray = (Player*) realloc(playersArray,numberOfPlayers * sizeof(Player));
	
	//Now we will ask the user to enter some data to identify him/her
	for (size_t i = 0; i < numberOfPlayers; i++)
	{
		while (1) {
			changeColor(RED);
			printf("\nPlease Player %d enter the pseudo you want to play with:\n", i+1);
			changeColor(YELLOW);
			getDataInput(userInput);

			if (pseudo_verification(userInput, playersArray, i + 1)) {
				//The player entered a correct pseudo
				break;
			}
			else {
				changeColor(RED);
				printf("\nThis pseudo is already taken... Find an other one!\n");
			}
		}

		playersArray[i].playeriD = i + 1;
		playersArray[i].penguinAmount = playersArray[i].playerPoints = 0;
		strcpy(playersArray[i].playerPseudo, userInput);
	}

	//We change the color to white
	changeColor(WHITE);
	
	//We clean the console
	system("cls");

	//We also return the number of players
	*numberPlayers = numberOfPlayers;

	return playersArray;
}

char * read_computer_pseudo()
{
	return "YouWillNotWin05";
}




