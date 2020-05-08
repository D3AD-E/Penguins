#include "designFunctions.h"
#include "boardFunction.h"
#include "Player.h"
#include "Movement.h"

#ifdef _WIN64
#include <Windows.h>
#endif

extern  int ROWS;
extern  int COLUMNS;

void changeColor(Color color) {
#ifdef _WIN64
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
#endif
}

#ifdef _WIN64
void main_menu(Tile** gameBoard, Player* playersArray)
{
	char characterEntered;
	COORD cursorPosition = { 0,0 };
	int lineNumber = 1;
	int numberOfPlayers;

	//We will first display the main menu with a focus on play a game
	changeParagraph(cursorPosition, 1);

	//We will wait until the user chose the quit program
	while (1) {
		//We will wait until the user taped on the enter key
		while (1) {
			while (_kbhit() == 0) {
				//We are waiting for the user to tap on a key
			}
			//We retrieve which key was taped
			characterEntered = _getch();
			characterEntered = characterEntered != -32 ? characterEntered : _getch(); //We do that two times if the key detected is an arrow key

			switch (characterEntered) {
			case 72:
				if (lineNumber > 1) lineNumber--;
				changeParagraph(cursorPosition, lineNumber);
				break;
			case 80:
				if (lineNumber < 4) lineNumber++;
				changeParagraph(cursorPosition, lineNumber);
				break;

			case 13:
				goto LINESELECTIONSTOP;

			default:
				break;
			}
		}
	LINESELECTIONSTOP:

		//Now we will execute the chosen function
		switch (lineNumber) {
		case 1:
			system("cls");

			/*We first need to ask how many players will play the game and then we have to change the
			the number of Columns and Rows depending on the number of players*/
			playersArray = register_players(playersArray, &numberOfPlayers);

			switch (numberOfPlayers) {
			case 2:
			case 3:
				ROWS = 6;
				COLUMNS = 6;
				break;

			case 4:
			case 5:
			case 6:
				ROWS = 8;
				COLUMNS = 8;
				break;

			case 7:
			case 8:
			case 9:
				ROWS = 10;
				COLUMNS = 10;
				break;
			}

			//We then generate our dynamic board;
			gameBoard = generate_board(gameBoard, numberOfPlayers);

			//We save the data concerning the players on the board file
			save_id_players_file(playersArray, numberOfPlayers);

			//We can now start the placement phase
			run_placement_stage(gameBoard, playersArray, numberOfPlayers);

			//We can now run movement phase
			penguin_move(gameBoard, INTERACTIVE, playersArray, numberOfPlayers);

			//Displaying results (who is the winner of the game)

			//We free the memory
			free_memory(playersArray, gameBoard);

			delete_board();
			changeParagraph(cursorPosition, 1);
			break;

		case 2:
			//We will read the documentation
			read_documentation();
			changeParagraph(cursorPosition, lineNumber);
			break;

		case 3:
			//We will load a game
			break;

		case 4:
			//We quit the program
			delete_board();
			goto ENDOFPROGRAM;
			break;

		default:
			break;
		}
	}
ENDOFPROGRAM:
	return;
}
#endif

#ifdef _WIN64
void changeParagraph(COORD position, int line_number)
{
	//We set the cursor to the position that we got previously
	setCursorPosition(0, 0);

	printf(" ----------------------\n");
	printf("|  HEY THAT'S MY FISH  |\n");
	printf(" ----------------------\n\n");

	switch (line_number) {
	case 1:
		changeColor(BLUE);
		printf("| Start a new game\n");
		changeColor(WHITE);
		printf("| Read documentation\n");
		printf("| Load a game (movement)\n");
		printf("| Quit the program\n\n");
		break;

	case 2:

		changeColor(WHITE);
		printf("| Start a new game\n");
		changeColor(BLUE);
		printf("| Read documentation\n");
		changeColor(WHITE);
		printf("| Load a game (movement)\n");
		printf("| Quit the program\n\n");
		break;

	case 3:
		changeColor(WHITE);
		printf("| Start a new game\n");
		printf("| Read documentation\n");
		changeColor(BLUE);
		printf("| Load a game (movement)\n");
		changeColor(WHITE);
		printf("| Quit the program\n\n");
		break;

	case 4:
		changeColor(WHITE);
		printf("| Start a new game\n");
		printf("| Read documentation\n");
		printf("| Load a game (movement)\n");
		changeColor(BLUE);
		printf("| Quit the program\n\n");
		changeColor(WHITE);
		break;

	default:
		break;
	}
}
#endif

#ifdef _WIN64
void read_documentation()
{
	FILE* file = NULL;
	char line[MAX_SIZE] = "";
	COORD beginConsole = { 0,0 };
	//Cleaning the console
	system("cls");

	//We will print the header
	printf(" -----------------------------------\n");
	printf("|  HEY THAT'S MY FISH DOCUMENTATION |\n");
	printf(" -----------------------------------\n\n");

	//We will now try to open the file explaining all the rules of the game

	if ((file = fopen("gameRules.txt", "r")) != NULL) {
		while (fgets(line, MAX_SIZE, file) != NULL) {
			puts(line);
		}
		//We close the file
		fclose(file);
	}
	else {
		fprintf(stderr, "An error occured while trying to open the file\n");
		exit(2);
	}

	printf("Please tap on enter key to return to the main menu...");

	//We scroll till the top of the screen buffer
	scrollToTop();

	while (_getch() != 13) {
	}

	system("cls");
}

void print_board(Tile** board)
{
	shift_cursor();
	//Printing the top of the board
	for (int k = 0; k < COLUMNS; k++)
	{
		if (k == 0)
			printf("/---");
		else
			printf("+---");
	}
	printf("\\");
	printf("\n");

	for (size_t i = 0; i < ROWS; i++)
	{
		shift_cursor();
		for (size_t j = 0; j < COLUMNS; j++)
		{
			if (board[i][j].idPlayer == 0 && board[i][j].fishNumber == 0) {
				printf("|*:*");
			}
			else {
				printf("|%d:%d", board[i][j].fishNumber, board[i][j].idPlayer);
			}
		}
		printf("|\n");
		shift_cursor();
		for (size_t k = 0; k < COLUMNS; k++)
		{
			//We detect if we are at the bottom of the board
			if (i == ROWS - 1) { goto BOARDEND; }
			printf("+---");
		}
		printf("+\n");
	}

BOARDEND:

	//We print the bottom of the board
	for (size_t i = 0; i < COLUMNS; i++)
	{
		if (i == 0) {
			printf("\\---");
		}
		else if (i == COLUMNS - 1) {
			printf("+---/\n");
		}
		else {
			printf("+---");
		}
	}
}

void shift_cursor()
{
	setCursorPosition(getCursorPosition().X + 2, getCursorPosition().Y);
}

void print_placement_phase()
{
	printf("===============================\n");
	printf("        PLACEMENT PHASE        \n");
	printf("===============================\n\n");
}

#ifdef _WIN64
void color_player_penguins(Tile** board, int idPlayer, COORD topBoardPosition, COORD bottomBoardPosition)
{
	COORD firstTilePosition;//correspond to the position of the first tile. It's a kind of marker
	COORD tileToColorPosition;
	firstTilePosition.X = topBoardPosition.X + 2;
	firstTilePosition.Y = topBoardPosition.Y + 1;

	//Now we are going to go through the board so as to retrieve tiles where there are player's penguins
	for (size_t i = 0; i < ROWS; i++)
	{
		for (size_t j = 0; j < COLUMNS; j++)
		{
			if (board[i][j].idPlayer == idPlayer) {
				tileToColorPosition.X = firstTilePosition.X + 4 * j;
				tileToColorPosition.Y = firstTilePosition.Y + 2 * i;

				color_tile(tileToColorPosition, board, i, j, 0);
			}
		}
	}

	setCursorPosition(bottomBoardPosition.X, bottomBoardPosition.Y);
}
#endif

#ifdef _WIN64
void color_tile(COORD tilePosition, Tile** board, int row, int column, int EmptyTile)
{
	//We change the color of the text into a pretty yellow
	if (EmptyTile == 1)
	{
		changeColor(RED);
	}
	else
	{
		changeColor(YELLOW);
	}

	//We print the first line of the tile
	setCursorPosition(tilePosition.X, tilePosition.Y - 1);
	if (row == 0 && column == 0) {
		printf("/---+");
	}
	else if (row == 0 && column == COLUMNS - 1) {
		printf("+---\\");
	}
	else
	{
		printf("+---+");
	}

	//We print the second line of the tile
	setCursorPosition(tilePosition.X, tilePosition.Y);

	if (EmptyTile == 1)
	{
		printf("|* *|", board[row][column].fishNumber, board[row][column].idPlayer);
	}
	else
	{
		printf("|%d:%d|", board[row][column].fishNumber, board[row][column].idPlayer);
	}

	//We print the third line of the tile
	setCursorPosition(tilePosition.X, tilePosition.Y + 1);
	if (row == ROWS - 1 && column == 0) {
		printf("\\---+");
	}
	else if (row == ROWS - 1 && column == COLUMNS - 1) {
		printf("+---/");
	}
	else
	{
		printf("+---+");
	}

	//We change the color of the text to its normal color: WHITE.
	changeColor(WHITE);
}
#endif

void print_movement_phase()
{
	printf("===============================\n");
	printf("         MOVEMENT PHASE        \n");
	printf("===============================\n\n");
}

void print_scores(Player* playersArray, int numberOfPlayers)
{
	printf("Nick\t ID \t Fish\n");
	for (int i = 0; i < numberOfPlayers; i++) {
		printf("%s\t %d\t %d\n", playersArray[i].playerPseudo, playersArray[i].playeriD, playersArray[i].playerPoints);
	}
	return;
}

void color_holes(Tile** board, COORD topBoardPosition, COORD bottomBoardPosition)
{
	COORD firstTilePosition;//correspond to the position of the first tile. It's a kind of marker
	COORD tileToColorPosition;
	firstTilePosition.X = topBoardPosition.X + 2;
	firstTilePosition.Y = topBoardPosition.Y + 1;

	//Now we are going to go through the board so as to retrieve tiles where there are player's penguins
	for (size_t i = 0; i < ROWS; i++)
	{
		for (size_t j = 0; j < COLUMNS; j++)
		{
			if (board[i][j].idPlayer == 0 && board[i][j].fishNumber == 0) {
				tileToColorPosition.X = firstTilePosition.X + 4 * j;
				tileToColorPosition.Y = firstTilePosition.Y + 2 * i;

				color_tile(tileToColorPosition, board, i, j, 1);
			}
		}
	}

	setCursorPosition(bottomBoardPosition.X, bottomBoardPosition.Y);
}

void suppress_line_buffer(COORD messageErrorPosition, short messageLen)
{
	setCursorPosition(messageErrorPosition.X, messageErrorPosition.Y);

	for (size_t i = 0; i < messageLen; i++)
	{
		fputc(' ', stdout);
	}

	setCursorPosition(0, 0);
}
#endif