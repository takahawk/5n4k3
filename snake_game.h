#ifndef SNAKE_GAME_H_
#define SNAKE_GAME_H_

#include "linked_list.h"
#include "matrix.h"

typedef struct {
	Matrix *field;
	LinkedList *snake;
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
SnakeGameTick(SnakeGame*);

void
FreeSnakeGame(SnakeGame*);
#endif
