#include <ncurses.h>
#include <unistd.h>

#include "../snake_game.h"

#define TICK_MS 100


int main() {
	initscr();
	int max_x, max_y;
	getmaxyx(stdscr, max_y, max_x);
	nodelay(stdscr, 1);

	SnakeGame *sg = AllocSnakeGame(max_x, max_y);

	Matrix *m = sg->field;
	int stop = 0;

	while (!stop) {
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

		int ch = getch();
		if (ch != ERR) {
			switch (ch) {
			case 'q':
				stop = 1;
				break;
			}
		}
		usleep(1000 * TICK_MS);
		SnakeGameTick(sg);	
	}

	FreeSnakeGame(sg);
	endwin();
	return 0;
}
