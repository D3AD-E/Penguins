#ifndef DESIGNFUNCTIONS_H
#define DESIGNFUNCTIONS_H

#include "Color.h"
#include "auxiliaryFunctions.h"
#include "ModeGame.h"
#include "Player.h"
#include "Tile.h"
#include <stdio.h>

#ifdef _WIN64
#include <Windows.h>
#include <conio.h>
#endif

#define MAX_SIZE 1000	

/*
changeColor: changes the color of the text in the console.

Entry:
	color: The new desired color: BLUE RED or WHITE.
*/
void changeColor(Color color);

/*
main_menu: displays the game's main menu. The user will be able to start a new game,
read the rules of the game, load a previous game and can also quit the program.

*/
void main_menu(Tile **gameBoard, Player *playersArray);

/*
changeParagraph: changes the color of the line in the main menu that the user needs to 
focus on. The user will be able to see to which part of the program, he or she will go
after tapping on the enter key.

Entry:
	position: position of the cursor before displaying the main menu.
	
	line_number: the line that needs to have its color changed.
*/

#ifdef _WIN64
void changeParagraph(COORD position, int line_number);
#endif

/*
read_documentation: read the rules of the game from a specific file and display them to
the user.
*/
void read_documentation();

/*
print_board: function that prints the game's board onto the console

Entry: game's board
*/
void print_board(Tile **board);

/*
shift_cursor: functions so as to shift the cursor so that the board is not closed to
the left side of the console
*/
void shift_cursor();

/*
print_placement_phase: function that just indicates that we are in the placement stage of
the game.
*/
void print_placement_phase();


/*
color_player_penguins: function that highlights the penguins of the player present on the
board so as to make it easier for him/her to determine where are his/her penguins.

Parameters:
	board: the board on which the players are playing
	idPlayer: the id that we are looking for on the board
	topBoardPosition: the position of the cursor before displaying the board
	bottomBoardPosition: the position of the cursor after displaying the board
*/

#ifdef _WIN64
void color_player_penguins(Tile** board, int idPlayer, COORD topBoardPosition, COORD bottomBoardPosition);
#endif

/*
color_tile: function that colors the tile in yellow (because on it there is the penguin
of the player making the placement or the movement

Parameters:
	tilePosition: position of the tile that we have to color
	board: the board on which the players are playing
	row: the row on which the tile to color is present
	column: the column on which the tile to color is present
*/

#ifdef _WIN64
void color_tile(COORD tilePosition, Tile **board, int row, int column, int EmptyTile);
#endif

/*
print_movement_phase: function that just indicates that we are in the movement stage of
the game.
*/
void print_movement_phase();


/*
Prints scores of players, thir nickanmes and their number
*/

void print_scores(Player *playersArray, int numberOfPlayers);


/*
Changes color of the empty tiles to red
*/

#ifdef _WIN64
void color_holes(Tile ** board, COORD topBoardPosition, COORD bottomBoardPosition);
#endif
/*
suppress_line_buffer: functions that suppress a line on the console
*/

#ifdef _WIN64
void suppress_line_buffer(COORD messageErrorPosition, short messageLen);
#endif


#endif // !DESIGNFUNCTIONS_H
