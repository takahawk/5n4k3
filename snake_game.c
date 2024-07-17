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
CheckBounds(SnakeGame *sg, IntVec2 *pos);

static void
PositionApple(SnakeGame *sg);

SnakeGame*
AllocSnakeGame(size_t w, size_t h, int addWalls) {
	SnakeGame *sg = malloc(sizeof(SnakeGame));
	sg->field = AllocMatrix(w, h, sizeof(SnakeGameObject));
	sg->snake = AllocLinkedList(sizeof(IntVec2));
	sg->addWalls = addWalls;

	srand(time(NULL));

	SnakeGameReset(sg);
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

	MatrixSetZeros(sg->field);
	memset(&sg->apple, 0, sizeof(IntVec2));
	int w = sg->field->w;
	int h = sg->field->h;
	
	if (sg->addWalls) {
		for (int i = 0; i < w; i++) {
			SetGameObject(sg, i, 0, WALL);
			SetGameObject(sg, i, h - 1, WALL);
		}

		for (int j = 0; j < h; j++) {
			SetGameObject(sg, 0, j, WALL);
			SetGameObject(sg, w - 1, j, WALL);
		}
	}

	LinkedListClear(sg->snake);
	IntVec2 snakePosition = { .x = sg->field->w / 2, .y = sg->field->h / 2 };
	LinkedListAdd(sg->snake, &snakePosition);
	MatrixSet(sg->field,
		  snakePosition.x, 
		  snakePosition.y, 
		  &((SnakeGameObject){SNAKE}));

	sg->direction = LEFT;


	PositionApple(sg);
}

void
SnakeGameTick(SnakeGame* sg) {
	if (sg->state == GAME_OVER) {
		return;
	}
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
	
	switch (GetGameObject(sg, newPos.x, newPos.y)) {
	case APPLE:
		LinkedListAdd(sg->snake, &newPos);
		PositionApple(sg);
		break;
	case WALL:
	case SNAKE:
		sg->state = GAME_OVER;
		break;
	case VOID:
		ListNode *node = sg->snake->head;

		IntVec2	exTailPos = *((IntVec2 *) node->val);
		while (node->next != NULL) {
			exTailPos = SwapNodeValue(node->next, exTailPos);
			node = node->next;
		}
		SetGameObject(sg, exTailPos.x, exTailPos.y, VOID);
		SetNodeValue(sg->snake->head, newPos);
		break;
	}
	
	SetGameObject(sg, newPos.x, newPos.y, SNAKE);
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
