#include <ncurses.h>

int main() {
	initscr();
	int max_x, max_y;
	getmaxyx(stdscr, max_x, max_y);
	
	box(stdscr, 0, 0);
	wrefresh(stdscr);
	getch();

	endwin();
	return 0;
}
