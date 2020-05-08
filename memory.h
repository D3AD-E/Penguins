#ifndef MEMORY_H
#define MEMORY_H

#include "Player.h"
#include "Position.h"
#include "Tile.h"
#include <stdio.h>

#ifdef _WIN64
#include <Windows.h>
#endif

/*
free_memory: functions that free the memory that we allocated for both the playersArray
and the board array.

Parameters:
	playersArray: a pointer to the array of players
	board: a pointer to our 2d array representing the board

Exit:
	1 ~ the memory was free successfully
	0 ~ an error occured
*/
void free_memory(Player* playersArray, Tile** board);

/*
remove_element: function that delete one of the element of an array.

PARAMATERS:
	array: array in which there is the element that we want to delete
	indexElement: the index of the element that we want to suppress.
*/
Position* remove_element(Position* arrayToModify, const int indexElement);

#endif // !MEMORY_H
