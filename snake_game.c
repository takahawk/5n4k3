#include "snake_game.h"

#include <stdlib.h>

#include "linked_list.h"
#include "matrix.h"

SnakeGame*
AllocSnakeGame(size_t w, size_t h) {
	SnakeGame *sg = malloc(sizeof(SnakeGame));
	sg->field = AllocMatrix(w, h);
	
	// TODO: position walls?
	sg->snake = AllocLinkedList();
	return sg;
}

void
SnakeGameTick(SnakeGame*) {
	// TODO: impl
}

void
FreeSnakeGame(SnakeGame* sg) {
	FreeMatrix(sg->field);
	FreeLinkedList(sg->snake);
	free(sg);
}
