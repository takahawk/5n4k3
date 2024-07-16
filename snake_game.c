#include "snake_game.h"

#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "linked_list.h"
#include "matrix.h"
#include "vec2.h"

inline static void
PositionApple(SnakeGame *sg);

inline static SnakeGameObject
GetGameObject(SnakeGame *sg, size_t x, size_t y);

inline static void
SetGameObject(SnakeGame *sg, size_t x, size_t y, SnakeGameObject obj);

SnakeGame*
AllocSnakeGame(size_t w, size_t h) {
	SnakeGame *sg = malloc(sizeof(SnakeGame));
	sg->field = AllocMatrix(w, h, sizeof(SnakeGameObject));
	sg->snake = AllocLinkedList(sizeof(IntVec2));
	memset(&sg->apple, 0, sizeof(IntVec2));
	
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
SnakeGameChangeDirection(SnakeGame *sg, SnakeDirection direction) {
	if (abs(direction - sg->direction) != 2)
		sg->direction = direction;
}

void
SnakeGameTick(SnakeGame* sg) {
	IntVec2 newPos = *((IntVec2 *) sg->snake->head->val);

	switch (sg->direction) {
	case UP:
		newPos.y -= 1;
		break;
	case DOWN:
		newPos.y += 1;
		break;
	case RIGHT:
		newPos.x += 1;
		break;
	case LEFT:
		newPos.x -= 1;
		break;
	}
	
	if (GetGameObject(sg, newPos.x, newPos.y) == APPLE) {
		LinkedListAdd(sg->snake, &newPos);
		PositionApple(sg);
	} else {
		ListNode *node = sg->snake->head;

		IntVec2	exTailPos = *((IntVec2 *) node->val);
		while (node->next != NULL) {
			IntVec2 t = exTailPos;
			exTailPos = *((IntVec2 *) node->next->val);
			memcpy(node->next->val, &t, sg->snake->elemSize);
			node = node->next;
		}
		SetGameObject(sg, exTailPos.x, exTailPos.y, VOID);
		memcpy(sg->snake->head->val, &newPos, sg->snake->elemSize);
	}

	SetGameObject(sg, newPos.x, newPos.y, SNAKE);

	// TODO: add bounds check
}

void
FreeSnakeGame(SnakeGame* sg) {
	FreeMatrix(sg->field);
	FreeLinkedList(sg->snake);
	free(sg);
}

inline static void
PositionApple(SnakeGame *sg) {
	if (GetGameObject(sg, sg->apple.x, sg->apple.y) == APPLE) {
		SetGameObject(sg, sg->apple.x, sg->apple.y, VOID);
	}
	do {
		sg->apple = (IntVec2) { .x = rand() % sg->field->w, .y = rand() % sg->field->h };
	} while (GetGameObject(sg, sg->apple.x, sg->apple.y) != VOID);
	MatrixSet(sg->field, sg->apple.x, sg->apple.y, &((int) {APPLE}));
}

inline static SnakeGameObject
GetGameObject(SnakeGame *sg, size_t x, size_t y) {
	return *((SnakeGameObject *) MatrixGet(sg->field, x, y));
}

inline static void
SetGameObject(SnakeGame *sg, size_t x, size_t y, SnakeGameObject obj) {
	MatrixSet(sg->field, x, y, &((int) { obj }));	
}
