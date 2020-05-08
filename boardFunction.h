#ifndef BOARDFUNCTION_H
#define BOARDFUNCTION_H

#include "auxiliaryFunctions.h"
#include "verificationFunctions.h"
#include "Tile.h"
#include "memory.h"
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <stdbool.h>
#include <time.h>

/*
generate_board: function that creates the initial state of the game: creation of the board,
placement of the fishes and it does the backup of the initial state into a specific file called
board.txt

Exit:
	the new board

*/
Tile** generate_board(Tile** gameBoard, int numberOfPlayers);

/*
fill_board: functions that places a certain amout of penguis on each square of
the board's game.

*/
void fill_board(Tile** board);

/*
save_board: function that permits to save the initial state of the board into a file called
board.txt.

Entry:
	board: it represents the game's board
Exit:
	an integer so as to know if the backup of the state was successfull or not 0 means ok and
	1 means that an error occured.
*/
int save_board(Tile** board, char* fileName);

/*
save_id_players_file is a function that permits to save the id of the players when we play
the game in interactive mode.

Entry:
	- userData: a two dimensional array containing all the information concerning the players
	- numberPlayers: the number of players to register in the file.
*/
void save_id_players_file(Player* userData, int numberPlayers);

/*
count_player_penguins: functions that will permit to count how many player's penguins are present
on the game's board at a current state of the game.

Entry:
	idPlayer: an integer that indicates the player's id for which we need to calculate the
	number of penguins currently present on the game's board.

Exit:
	the number of penguins that are on the board for a specific player.

*/
int count_player_penguins(int idPlayer);

/*
read_players_id: functions which allows to display the id of the players

*/
void read_players_id();

/*
delete_board: function that deletes the board file at the end of the game.
*/
void delete_board();

/*
register_special_player: functions that will register the computer (special player) if it's not
already done in the board file with a nonce number.

Entry:
	idComputer: the number to identify our program on the board file
	pseudoComputer: the pseudo used by our computer
	boardName: the name of the file on which we will register our computer
*/
void register_special_user(int idComputer, char* pseudoComputer, char* boardName);

/*
read_board_data: functtion that reads the current state of the board's game

Entry:
	board: the current game's board that we will fill
*/
void read_board_data(Tile** board, char* fileName);

void penguin_removal(Tile** board, int PlayerNumL); //REMOVES A PENGUIN IF IT IS NOT ABLE TO MOVE

/*
changing_board: function that will fill the board once again so as to fit the number of tiles
containing only one penguin needed

Parameters:
	board: the board on which the players will play
*/
void change_board(Tile** board, int missingNumberOfTiles);

/*
read_board_size: functions that determines the size of the board.
*/

void read_board_size(char* nameFile);

/*
update_board_movement: function that will effectuate the movement

PARAMETERS:
	board ~ board game
	penguinInitialPosition ~ the initial position of the penguin
	penguinFinalPosition ~ the final position of the penguin
	fishEarned ~ the number of fish collected during this turn
	idComputer ~ the number that the computer received during registration to identify it on the board
*/
void update_board_movement(Tile** board, const Position initialPosition, const Position finalPosition, int* numberFishCollected, int idComputer);

/*
determine_computer_iD: function that determine the id that our computer will need to use
so as to identify it on the board.

Entry:
	boardFileName: the name of the board's file

Output:
	the id that our computer will need to use.
*/
int determine_computer_iD(char* boardFileName);

/*
update_points: function that updates the board's file after a movement turn. We will add
the number of fish earned by the program during its turn

Entry:
	newValue: the number of fish that the computer owns at the current time
*/
void update_points(char* boardFileName, int iDComputer, int newValue);

/*
read_computer_id: function that reads the computer id on the board file
*/
int read_computer_id(char* boardFileName);
#endif // !BOARDFUNCTION_H
