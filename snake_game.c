#include "snake_game.h"

#include <stdlib.h>

#include "linked_list.h"
#include "matrix.h"
#include "vec2.h"

SnakeGame*
AllocSnakeGame(size_t w, size_t h) {
	SnakeGame *sg = malloc(sizeof(SnakeGame));
	sg->field = AllocMatrix(w, h, sizeof(SnakeGameObject));
	sg->snake = AllocLinkedList();
	
	// TODO: position snake?
	IntVec2 snakePosition = { .x = w / 2, .y = h / 2 };
	LinkedListAdd(sg->snake, &snakePosition);
	MatrixSet(sg->field,
		  snakePosition.x, 
		  snakePosition.y, 
		  &((SnakeGameObject){SNAKE}));
	// TODO: position apple?
	// TODO: position walls?
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
