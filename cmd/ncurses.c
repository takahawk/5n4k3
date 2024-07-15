#include <ncurses.h>

#include "../snake_game.h"

int main() {
	initscr();
	int max_x, max_y;
	getmaxyx(stdscr, max_y, max_x);

	SnakeGame *sg = AllocSnakeGame(max_x, max_y);
	Matrix *m = sg->field;

	for (int x = 0; x < m->w; x++) {
		for (int y = 0; y < m->h; y++) {
			SnakeGameObject *o = MatrixGet(m, x, y);
			char c;
			switch (*o) {
			case SNAKE:
				c = 'S';	
				break;
			case APPLE:
				c = '@';
				break;
			case WALL:
				c = 'x';
				break;
			default:
				c = ' ';
				break;
			}
			mvaddch(y, x, c);
		}
	}
	
	box(stdscr, 0, 0);
	wrefresh(stdscr);
	getch();

	FreeSnakeGame(sg);
	endwin();
	return 0;
}
