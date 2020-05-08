#include "Movement.h"

extern const int ROWS;
extern const int COLUMNS;





int penguin_move(Tile ** board, ModeGame game_mode, Player *playersArray, int numberOfPlayers)
{
	#ifdef _WIN64
	COORD topBoardPosition;
	COORD bottomBoardPosition;
	#endif

	Position desiredTile;
	bool Success = false;
	bool Fail;
	bool isPenguins;
	int y = 0, x = 0, NewX, NewY;
	int counter = 0;

	if (game_mode == INTERACTIVE) {
		#ifdef _WIN64
		while (1) 
		{
			for (int a = 0; a < numberOfPlayers; a++) {
				Fail = false;
				isPenguins = false;
				counter = 0;
				Success = false;
				for (int j = 0; j < ROWS; j++)
				{
					for (int i = 0; i < COLUMNS; i++)
					{
						if (board[j][i].idPlayer == playersArray[a].playeriD)
							counter++;
						for (int b = 0; b < numberOfPlayers; b++)
						{
							if (board[j][i].idPlayer == playersArray[b].playeriD)
								isPenguins = true;
						}
					}
				}
				if (isPenguins == false)
					break;
				if (counter == 0)
					Success = true;
				while (Success == false)
				{
					system("cls");
					changeColor(WHITE);
					print_movement_phase();
					topBoardPosition = getCursorPosition();
					print_board(board);
					bottomBoardPosition = getCursorPosition();
					color_player_penguins(board, playersArray[a].playeriD, topBoardPosition, bottomBoardPosition);

					//We color the empty tiles
					color_holes(board, topBoardPosition, bottomBoardPosition);

					

					setCursorPosition(bottomBoardPosition.X, bottomBoardPosition.Y + 2);
					printf("------------------------------------------------------\n");

					print_scores(playersArray, numberOfPlayers);

					printf("Choose the penguin location(%s).", playersArray[a].playerPseudo);

					Fail = false;
					desiredTile = get_coordinate_desired_tile(topBoardPosition, bottomBoardPosition,NULL);
					y = desiredTile.rowNumber;
					x = desiredTile.columnNumber;
					if (board[y][x].idPlayer != playersArray[a].playeriD)//ERROR PENGUIN NOT FOUND AT LOCATION
					{
						changeColor(RED);
						printf("There is no penguin at that location. Try again.\n");
						Sleep(2000);
					}
					else//MOVING A PENGUIN
					{
						system("cls");
						changeColor(WHITE);
						//setCursorPosition(0, 0);
						print_movement_phase();
						topBoardPosition = getCursorPosition();
						print_board(board);
						bottomBoardPosition = getCursorPosition();
						color_player_penguins(board, playersArray[a].playeriD, topBoardPosition, bottomBoardPosition);
						color_holes(board, topBoardPosition, bottomBoardPosition);

						setCursorPosition(bottomBoardPosition.X, bottomBoardPosition.Y + 2);

				

						printf("------------------------------------------------------\n");
						print_scores(playersArray, numberOfPlayers);


						printf("Move to point.");
						COORD pos = { x,y };
						desiredTile = get_coordinate_desired_tile(topBoardPosition, bottomBoardPosition, &pos);

						NewX = desiredTile.columnNumber;
						NewY = desiredTile.rowNumber;
						if (NewX != x && NewY != y)
							//ERROR WRONG LINE
						{
							changeColor(RED);
							printf("You can move your penguin only in a straight line. Try again.\n");
							Sleep(2000);
						}
						else if ((board[NewY][NewX].idPlayer != 0) || (board[NewY][NewX].fishNumber == 0))
							//EROOR TRING TO PUT AT LOCATION WHERE THERE IS A HOLE OR A PENGUIN
						{
							changeColor(RED);
							printf("You can put you penguin only on a tile with fish and without penguins. Try again.\n");
							Sleep(2000);
						}
						else
						{
							if (x != NewX)
							{
								for (int xi = x + 1; xi < NewX; xi++)
									if ((board[y][xi].idPlayer != 0) || (board[y][xi].fishNumber == 0))
										Fail = true;
								for (int xi = x - 1; xi > NewX; xi--)
									if ((board[y][xi].idPlayer != 0) || (board[y][xi].fishNumber == 0))
										Fail = true;
							}
							else
							{

								for (int yi = y + 1; yi < NewY; yi++)
									if ((board[yi][x].idPlayer != 0) || (board[yi][x].fishNumber == 0))
										Fail = true;
								for (int yi = y - 1; yi > NewY; yi--)
									if ((board[yi][x].idPlayer != 0) || (board[yi][x].fishNumber == 0))
										Fail = true;
							}
							if (Fail)
								//ERROR SOMETHING IS STAYING BETWEEN YOUR PENGUIN AND TARGET LOCATION
							{
								changeColor(RED);
								printf("Something is staying between your penguin and target location. Try again.\n");
								Sleep(2000);
							}
							else
								//PLACING A PENGUIN AND MAKING A HOLE (00) AND REMOVING PENGUIN FROM PREVIOUS LOCATION
							{
								board[NewY][NewX].idPlayer = playersArray[a].playeriD;
								board[y][x].idPlayer = 0;
								playersArray[a].playerPoints += board[NewY][NewX].fishNumber;
								board[NewY][NewX].fishNumber = 0;
								changeColor(YELLOW);
								printf("Penguin placed \n");
								changeColor(WHITE);
								Sleep(2000);
								for (size_t i = 1; i <= numberOfPlayers; i++)
								{
									penguin_removal(board, i);
								}
								Success = true;
							}
						}
					}
				}
				if (isPenguins == false)
					break;
			}

			if (isPenguins == false)
				break;
		}
		//We clean the console
		system("cls");
		int temp;
		print_scores(playersArray, numberOfPlayers);
		temp = playersArray[0].playerPoints;
		for (int a = 0; a < numberOfPlayers; a++)
		{
			if (playersArray[a].playerPoints > temp)
				temp = playersArray[a].playerPoints;
		}
		for (int a = 0; a < numberOfPlayers; a++)
		{
			if (playersArray[a].playerPoints == temp)
				printf("Congratulations %s YOU WON with %d points\n", playersArray[a].playerPseudo, temp);
		}
		printf("Press esc to go to main menu\n");

		while (1) {
			char keyTapped;
			while (!_kbhit())
			{
				//We are waiting for the user to tap on a valid key
			}


			keyTapped = _getch();
			keyTapped = keyTapped != -32 ? keyTapped : _getch(); //We do that two times if the key detected is an arrow key

			if (keyTapped == 27)
			{
				system("cls");
				break;
			}
		}
		return 0;
		#endif
	}
	else {
		//Automatic phase
		return ai_movement(board, playersArray->playeriD, &playersArray->playerPoints);
	}
}

