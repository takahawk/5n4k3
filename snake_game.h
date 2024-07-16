#ifndef SNAKE_GAME_H_
#define SNAKE_GAME_H_

#include "linked_list.h"
#include "matrix.h"
#include "vec2.h"

typedef enum {
	UP, RIGHT, DOWN, LEFT
} SnakeDirection;

typedef enum {
	RUNNING,
	GAME_OVER
} SnakeGameState;

typedef struct {
	Matrix *field;
	LinkedList *snake;
	SnakeDirection direction;
	IntVec2 apple;
	SnakeGameState state;
} SnakeGame;

typedef enum {
	VOID,
	SNAKE,
	APPLE,
	WALL
} SnakeGameObject;

SnakeGame*
AllocSnakeGame(size_t w, size_t h, int addWalls);

void
SnakeGameChangeDirection(SnakeGame*, SnakeDirection);

void
SnakeGameReset(SnakeGame*);

void
SnakeGameTick(SnakeGame*);

void
FreeSnakeGame(SnakeGame*);
#endif
