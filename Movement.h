#ifndef MOVEMENT_H_INCLUDED
#define MOVEMENT_H_INCLUDED

#include <stdbool.h>
#include "ModeGame.h"
#include "designFunctions.h"
#include "boardFunction.h"





// MOVES PENGUINS FROM TILE TO TILE BY USER INPUT
int penguin_move(Tile ** board, ModeGame game_mode, Player *playersArray, int numberOfPlayers);



#endif // MOVEMENT_H_INCLUDED