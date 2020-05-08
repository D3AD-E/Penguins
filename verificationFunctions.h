#ifndef VERIFICATIONFUNCTIONS_H
#define VERIFICATIONFUNCTIONS_H

#include "Player.h"
#include "Tile.h"
#include "boardFunction.h"
#include <string.h>
#include <stdio.h>

#ifdef _WIN64
#include <Windows.h>
#endif
/*
numberVerification: fonction that verify if the data entered as a parameter corresponds
to a number or not.

Entry:
	data: the data to test

Exit:
	an integer 0 if the string corresponds to a number 1 otherwise
*/
int numberVerification(char data[500]);

/*
pseudo_verification: functions that determine if a pseudo is already used by an other player

Parameters:
	pseudoToTest: the pseudo that a player wants to use
	currentPlayersList: The list containing all the players registered for the moment so as
	to play a game

Exit:
	1 ~ Indicates that the pseudo can be use
	0 ~ Indicates that the pseudo is already taken
*/
int pseudo_verification(char* pseudoToTest, Player* currentPlayersList, int numberPlayersCurrentlyRegistered);

/*
board_verification: functions that permits to verify if the head master created / launched
the .exe command so as to create a board or not.

Exit:
	1 means that the file exits
	0 means that no board has been created
*/
int board_verification();

/*
filling_board_verification: functions that analyses the board created. Indeed we need to
verify  if there are a sufficient number of tiles with only one fish on it.

Parameters:
	board: the board used for the game
*/
void filling_board_verification(Tile** board, int numberOfPlayers);

/*
check_movement_possible: function that determines if a penguin can effectuate a movement or not

PARAMETERS:
	board ~ board on which we are playing
	penguinPosition ~ position for which we want to know if a movement is possible or not

OUTPUT:
	movementCode: 1 ~ movement possible | 0 ~ movement not possible
*/
int check_movement_possible(Tile** board, const Position penguinPosition);

/*
computer_registered_on_file: function that determines if our computer is register on the
board file or not.

Entry:
	boardFileName: the name of the file for which we need to verify the presence of our
	computer's iD.
*/
short computer_registered_on_file(char* boardFileName);

#endif // VERIFICATIONFUNCTIONS_H
