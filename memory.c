#include "memory.h"

void free_memory(Player* playersArray, Tile** board)
{
	if (playersArray != NULL) {
		free(playersArray);
	}

	if (board != NULL) {
		for (size_t i = 0; i < (_msize(board) / sizeof(Tile*)); i++)
		{
			free(board[i]);
		}
		free(board);
	}
}

Position* remove_element(Position* arrayToModify, const int indexElement)
{
	Position* newArray;
	int countElement = 0;

	newArray = (Position*)calloc((_msize(arrayToModify) / sizeof(Position)) - 1, sizeof(Position));

	for (size_t i = 0; i < _msize(arrayToModify) / sizeof(Position); i++)
	{
		if (i != indexElement) {
			countElement++;

			newArray[countElement - 1] = arrayToModify[i];
		}
	}

	free(arrayToModify);

	return newArray;
}