#include "snake_game.h"

#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "linked_list.h"
#include "matrix.h"
#include "vec2.h"

inline static SnakeGameObject
GetGameObject(SnakeGame *sg, size_t x, size_t y) {
	return *((SnakeGameObject *) MatrixGet(sg->field, x, y));
}

inline static void
SetGameObject(SnakeGame *sg, size_t x, size_t y, SnakeGameObject obj) {
	MatrixSet(sg->field, x, y, &((int) { obj }));	
}

inline static IntVec2
GetNodeValue(ListNode *ln) {
	return *((IntVec2 *) ln->val);
}

inline static void
SetNodeValue(ListNode *ln, IntVec2 val) {
	memcpy(ln->val, &val, sizeof(IntVec2));
}

inline static IntVec2
SwapNodeValue(ListNode *ln, IntVec2 newVal) {
	IntVec2 oldVal = GetNodeValue(ln);
	SetNodeValue(ln, newVal);
	return oldVal;
}

static void
PositionApple(SnakeGame *sg);

static void
CheckBounds(SnakeGame *sg, IntVec2 *pos);

SnakeGame*
AllocSnakeGame(size_t w, size_t h) {
	SnakeGame *sg = malloc(sizeof(SnakeGame));
	sg->field = AllocMatrix(w, h, sizeof(SnakeGameObject));
	sg->snake = AllocLinkedList(sizeof(IntVec2));
	
	SnakeGameReset(sg);
	// TODO: position walls?
	return sg;
}

void
SnakeGameChangeDirection(SnakeGame *sg, SnakeDirection direction) {
	if (abs(direction - sg->direction) != 2)
		sg->direction = direction;
}

void
SnakeGameReset(SnakeGame* sg) {
	sg->state = RUNNING;
	memset(&sg->apple, 0, sizeof(IntVec2));
	
	IntVec2 snakePosition = { .x = sg->field->w / 2, .y = sg->field->h / 2 };
	LinkedListAdd(sg->snake, &snakePosition);
	MatrixSet(sg->field,
		  snakePosition.x, 
		  snakePosition.y, 
		  &((SnakeGameObject){SNAKE}));

	sg->direction = LEFT;

	srand(time(NULL));

	PositionApple(sg);
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
	CheckBounds(sg, &newPos);
	
	if (GetGameObject(sg, newPos.x, newPos.y) == APPLE) {
		LinkedListAdd(sg->snake, &newPos);
		PositionApple(sg);
	} else {
		ListNode *node = sg->snake->head;

		IntVec2	exTailPos = *((IntVec2 *) node->val);
		while (node->next != NULL) {
			exTailPos = SwapNodeValue(node->next, exTailPos);
			node = node->next;
		}
		SetGameObject(sg, exTailPos.x, exTailPos.y, VOID);
		SetNodeValue(sg->snake->head, newPos);
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

static void
PositionApple(SnakeGame *sg) {
	if (GetGameObject(sg, sg->apple.x, sg->apple.y) == APPLE) {
		SetGameObject(sg, sg->apple.x, sg->apple.y, VOID);
	}
	do {
		sg->apple = (IntVec2) { .x = rand() % sg->field->w, .y = rand() % sg->field->h };
	} while (GetGameObject(sg, sg->apple.x, sg->apple.y) != VOID);
	MatrixSet(sg->field, sg->apple.x, sg->apple.y, &((int) {APPLE}));
}

static void
CheckBounds(SnakeGame *sg, IntVec2 *pos) {
	if (pos->x < 0) {
		pos->x = sg->field->w - 1;
	}
	if (pos->x >= sg->field->w) {
		pos->x = 0;
	}

	if (pos->y < 0) {
		pos->y = sg->field->h - 1;
	}
	if (pos->y >= sg->field->h) {
		pos->y = 0;
	}
}
