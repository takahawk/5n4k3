#include <ncurses.h>

#include "../matrix.h"

int main() {
	initscr();
	int max_x, max_y;
	getmaxyx(stdscr, max_x, max_y);

	Matrix *m = AllocMatrix(max_y, max_x, sizeof(char));
	MatrixSet(m, 10, 10, &(char){'A'});
	MatrixSet(m, 10, 11, &(char){'B'});
	MatrixSet(m, 11, 10, &(char){'C'});
	MatrixSet(m, 11, 11, &(char){'D'});

	for (int x = 0; x < m->w; x++) {
		for (int y = 0; y < m->h; y++) {
			char *c = MatrixGet(m, x, y);
			mvaddch(y, x, *c);
		}
	}
	
	box(stdscr, 0, 0);
	wrefresh(stdscr);
	getch();

	FreeMatrix(m);
	endwin();
	return 0;
}
