#include "snake_game.h"

#include <stdlib.h>
#include <time.h>

#include "linked_list.h"
#include "matrix.h"
#include "vec2.h"

inline static void
PositionApple(SnakeGame *sg);

SnakeGame*
AllocSnakeGame(size_t w, size_t h) {
	SnakeGame *sg = malloc(sizeof(SnakeGame));
	sg->field = AllocMatrix(w, h, sizeof(SnakeGameObject));
	sg->snake = AllocLinkedList();
	
	IntVec2 snakePosition = { .x = w / 2, .y = h / 2 };
	LinkedListAdd(sg->snake, &snakePosition);
	MatrixSet(sg->field,
		  snakePosition.x, 
		  snakePosition.y, 
		  &((SnakeGameObject){SNAKE}));

	sg->direction = LEFT;

	srand(time(NULL));

	PositionApple(sg);
	
	// TODO: position walls?
	return sg;
}

void
SnakeGameTick(SnakeGame* sg) {
	// TODO: impl
}

void
FreeSnakeGame(SnakeGame* sg) {
	FreeMatrix(sg->field);
	FreeLinkedList(sg->snake);
	free(sg);
}

inline static void
PositionApple(SnakeGame *sg) {
	sg->apple = (IntVec2) { .x = rand() % sg->field->w, .y = rand() % sg->field->h };
	MatrixSet(sg->field, sg->apple.x, sg->apple.y, &((int) {APPLE}));
}
