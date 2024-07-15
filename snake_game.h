#ifndef SNAKE_GAME_H_
#define SNAKE_GAME_H_

#include "linked_list.h"
#include "matrix.h"
#include "vec2.h"

typedef enum {
	UP, RIGHT, DOWN, LEFT
} SnakeDirection;

typedef struct {
	Matrix *field;
	LinkedList *snake;
	SnakeDirection direction;
	IntVec2 apple;
} SnakeGame;

typedef enum {
	VOID,
	SNAKE,
	APPLE,
	WALL
} SnakeGameObject;

SnakeGame*
AllocSnakeGame(size_t w, size_t h);

void
SnakeGameChangeDirection(SnakeGame*, SnakeDirection);

void
SnakeGameTick(SnakeGame*);

void
FreeSnakeGame(SnakeGame*);
#endif
