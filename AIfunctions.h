#ifndef AIFUNCTIONS_H
#define AIFUNCTIONS_H

#include "Position.h"
#include "Tile.h"
#include "verificationFunctions.h"
#include "designFunctions.h"
#include <stdlib.h>
#include <stdio.h>


int ai_placement(Tile **board, Player *currentPlayer);

/*
ai_placement: function that will place automaticlly a penguin on board's tile
during placement phase. Firstly, it will choose row (or column) with the biggest number
of fish and then randomly choose proper tile.

PARAMETERS:
    board - the board on which the game is being played
    currentPlayer - our program's player structure
    
OUTPUT:
    1 - penguin has been placed
*/


/*
ai_movement: functions that will allow the computer to select a tile during
movement phase. This tile will be selected according to the fact that the number of fish
on the tile is a great value (the best would be 3) but also according to the number of fish
on its neighbooring tiles

PARAMETERS:
	board ~ the board on which the computer is playing
	idComputer ~ the number id that identify our computer on the board 
	desiredTile ~ pointor to an eventual desired tile

OUTPUT:
	a code of validation ~ 0 means that a movement was done by one of the penguin.
						 ~ 1 means that the computer cannot move any penguin
*/



/*
ai_movement: functions that will allow the computer to select a tile during
movement phase. This tile will be selected according to the fact that the number of fish
on the tile is a great value (the best would be 3) but also according to the number of fish
on its neighbooring tiles

PARAMETERS:
	board ~ the board on which the computer is playing
	idComputer ~ the number id that identify our computer on the board 
	desiredTile ~ pointor to an eventual desired tile

OUTPUT:
	a code of validation ~ 0 means that a movement was done by one of the penguin.
						 ~ 1 means that the computer cannot move any penguin
*/
int ai_movement(Tile **board, int idComputer, int *fishEarned);


/*
retrieve_position_best_tiles: function that give an array containing the position of the
tiles containing a certain amount of fish (the best is 3).

PARAMETERS:
	board ~ the board on which we are playing
	currentTilePos ~ the position of the penguin to evaluate his movement.
	numberFish ~ the number of fish that we are looking for on a tile

OUTPUT:
	an array containing all the position retrieved by the algorithm for a specific penguin.
*/
Position* retrieve_position_best_tiles(const Tile **board, const Position currentTilePos, const int numberFish);



/*
compute_tile_points: function that will attribute a certain amount of points to a given
tile

PARAMETERS:
	board ~ the board game
	tilePosition ~ the tile for which we need to compute a certain amount of points

OUTPUT:
	the points attributed to this tile.
*/
int compute_tile_points(const Tile **board, Position tilePosition);


/*
determine_best_tile: function that will select tile with the highest value of points or the last
tile found with the highest value of points.


PARAMETERS:
	bestTile: the tile with the best configuration (pointor)
	specificTiles: the array containing the specific positions
	tilesPoint: the array containing all the points for each potential penguins' tile

OUTPUT:
	the position of the best tile
*/
Position determine_best_tile(const Position** specificTiles, const int **tilePoint, int *indexInitialPosition);
#endif // !AIFUNCTIONS_H

