#ifndef AUXILIARYFUNCTIONS_H
#define AUXILIARYFUNCTIONS_H


#include "designFunctions.h"
#include "Position.h"
#include "boardFunction.h"
#include "verificationFunctions.h"
#include "Movement.h"
#include "GamePhase.h"
#include "ModeGame.h"
#include "Player.h"
#include "memory.h"
#include "Tile.h"
#include "AIfunctions.h"
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <string.h>

#ifdef _WIN64
#include <Windows.h>
#endif


#include <setjmp.h>



/*
	play_game: functions that will permit to interact with the current mode in both interactive
	or automatic mode

	Exit:
		1 ~ Movement impossible or placement done
		0 ~ Placement or movement done with success
*/
int play_game(int argc, char* argv[],Tile **gameBoard, Player *playersArray);


/*
	detect_game_phase: functions that permits to determine if we are in placement or movement phase

	Entry:
		phase: a string which looks like this "phase=movement"
	Exit:
		0 ~ Movement Phase
		1 ~ Placement Phase

*/
int detect_game_phase(char *phase);






/*
retrieve_players_data: function that permits to collect the username of each player 
in an array.

Exit:
	Array containing the username of each player.

*/
char** retrieve_players_data();



/*
getDataInput: function that allows to retrieve data given by the user and avoid a buffer
overflow attack.

entry:
	data: the string where the data will be stored
exit:
	an integer so as to know if we could retrieve the data
*/
int getDataInput(char data[500]);


/*
setCursorPosition: function that permits to set the new coordinate of the cursor in the
console

Entry:
	X: horizontal coordinate
	Y: vertical coordinate
*/
void setCursorPosition(int X, int Y);

/*
getCursorPosition: function that permits to retrieve the current position of the cursor
in the console.

Exit:
	Current position of the cursor in the console.
*/

#ifdef _WIN64
COORD getCursorPosition();
#endif
/*
scrollToTop: functions that will permit after reading the rules file to scroll till the top
of the screen buffer. Thanks to that, it will be easier for the user to read all the rules.
*/
void scrollToTop();

/*
get_coordinate_desired_tile: permits to know which tile the current player would like to
move his/her selected penguin.

Entry:
	topBoardPosition: the position of the cursor before displaying the board
	bottomBoardPosition: the position of the cursor after displaying the board

Exit:
	the position of the selected tile.
*/
#ifdef _WIN64
Position get_coordinate_desired_tile(COORD topBoardPosition, COORD bottomBoardPosition, COORD* desiredTilePosition);
#endif




/*
find_id_player: function that permits to find the id of a player from the data read from the 
file

Entry:
	data: ID PLAYER ~ ID NUMBER ~ NUMBER OF POINTS
EXIT:
	the id of the player
*/
char* find_id_player(char* data);


/*
penguin_placement: function that will place a penguin on the board for a specific player

Entry: 
	idPlayer: the id of the player placing the penguin
	maxPenguins: the maximum number of penguin that can be placed on the board
	board: the game board

Exit:
	1: means that the maximum number of penguins placed on the board has been reach
	0: placement done with success
*/
int penguin_placement(int maxPenguins, Player *currentPlayer, Tile ** board, ModeGame game_mode);


/*
run_placement_stage: functions that runs the placement stage (it's similar to the script
used for automatic mode by the game master

Parameters:
	board: the board on which the players will place their penguins
	numberOfPlayers: the number of players that will place penguins on the board
*/
void run_placement_stage(Tile **board, Player *playersArray,int numberOfPlayers);


/*
retrieve_max_penguins: function that permits to detect what is the maximum number of
penguins to place (USED ONLY FOR AUTOMATIC MODE):

Entry:
	data: command line corresponding to penguins=x

Exit:
	the maximum of penguins to place
*/
int retrieve_max_penguins(char *data);


/*
register_players : functions that will permit to determine the number of players who
will play a game.

Parameter:
	playersArray: the array where will be stored the data concerning players
	numberPlayers: the number of players that will play the game.
Exit:
	the array containing player's data
*/
Player* register_players(Player *playersArray, int *numberPlayers);


/*
read_computer_pseudo: function that return the pseudo of the computer
*/
char* read_computer_pseudo();

#endif // !AUXILIARYFUNCTIONS_H
