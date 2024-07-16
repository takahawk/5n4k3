#include <ctype.h>
#include <errno.h>
#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/param.h>
#include <unistd.h>

#include "../snake_game.h"

#define TICK_MS 100


typedef struct {
	int width;
	int height;
	int addWalls;
} Options;

typedef struct {
	int maxWidth;
	int maxHeight;
} SystemInfo;

void
handleOptions(int argc, char **argv, SystemInfo sysInfo, Options *opts);

int main(int argc, char **argv) {
	SystemInfo sysInfo;
	Options opts;
	WINDOW *borderWindow;
	WINDOW *w;
	int termW, termH;
	
	initscr();
	getmaxyx(stdscr, termH, termW);
	sysInfo = (SystemInfo) {
		.maxHeight = termH - 2,
		.maxWidth = termW - 2
	};
	
	handleOptions(argc, argv, sysInfo, &opts);
	nodelay(stdscr, 1);
	noecho();
	
	int starty = (termH - opts.height) / 2;
	int startx = (termW - opts.width) / 2;
	
	w = newwin(opts.height, opts.width, starty, startx);
	borderWindow = newwin(opts.height, opts.width, starty, startx);

	SnakeGame *sg = AllocSnakeGame(opts.width, opts.height, opts.addWalls);

	Matrix *m = sg->field;
	int stop = 0;

	while (!stop) {
		for (int x = 0; x < m->w; x++) {
			for (int y = 0; y < m->h; y++) {
				SnakeGameObject *o = MatrixGet(m, x, y);
				char c;
				switch (*o) {
				case SNAKE:
					c = 'x';
					break;
				case APPLE:
					c = '@';
					break;
				case WALL:
					c = '#';
					break;
				default:
					c = ' ';
					break;
				}
				mvwaddch(w, y, x, c);
			}
		}
		
		// TODO: fix displaying borders
		box(borderWindow, 0, 0);

		wrefresh(w);

		int ch = getch();
		if (ch != ERR) {
			switch (ch) {
			case 'q':
				stop = 1;
				break;
			case 'h':
				SnakeGameChangeDirection(sg, LEFT);
				break;
			case 'j':
				SnakeGameChangeDirection(sg, DOWN);
				break;
			case 'k':
				SnakeGameChangeDirection(sg, UP);
				break;
			case 'l':
				SnakeGameChangeDirection(sg, RIGHT);
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

void
handleOptions(int argc, char **argv, SystemInfo sysInfo, Options *opts) {
	int c;

	// defaults
	int defSize = MIN(sysInfo.maxWidth, sysInfo.maxHeight);
	opts->width = defSize; 
	opts->height = defSize;

	while ((c = getopt(argc, argv, "Ww:h:")) != -1) {
		switch (c) {
		case 'W':
			opts->addWalls = 1;
			break;
		case 'w':
			int w = strtol(optarg, NULL, 10);
			if (errno == EINVAL) {
				fprintf(stderr, "width should be integer");
				exit(-1);
			}

			if (w <= 0) {
				fprintf(stderr, "width should be more than zero");
				exit(-1);
			}

			if (w > sysInfo.maxWidth) {
				fprintf(stderr, "width is more than maximum terminal screen width");
				exit(-1);
			}

			opts->width = w;
			
			break;
		case 'h':
			int h = strtol(optarg, NULL, 10);
			if (errno == EINVAL) {
				fprintf(stderr, "height should be integer");
				exit(-1);
			}

			if (h <= 0) {
				fprintf(stderr, "height should be more than zero");
				exit(-1);
			}

			if (h > sysInfo.maxWidth) {
				fprintf(stderr, "height is more than maximum terminal screen height");
				exit(-1);
			}

			opts->height = h;

			break;
		case '?':
			if (optopt == 'w' || optopt == 'h') {
				fprintf(stderr, "Options -w and -h requires arguments");
				exit(-1);
			}
			if (isprint(optopt)) {
				fprintf(stderr, "Unknown option -%c", optopt);
				exit(-1);
			} else {
				fprintf(stderr, "Illegal option character");
				exit(-1);
			}
		default:
			abort();
		}
	}
}
