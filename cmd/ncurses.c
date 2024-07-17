#include <ctype.h>
#include <errno.h>
#include <math.h>
#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/param.h>
#include <sys/time.h>
#include <unistd.h>

#include "../snake_game.h"

#define TICK_MS 100
#define DEATH_BLINK_FREQ 100.
#define TIME_TO_RESET 2000.

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

long
timeMs() {
	struct timeval tp;
	gettimeofday(&tp, NULL);

	return tp.tv_sec * 1000 + tp.tv_usec / 1000.;
}

void
printUsage() {
	printf("Usage: 5n4k3 [OPTIONS]\n\n");
	printf("Options:\n");
	printf("\t-w <NUMBER>: set width\n");
	printf("\t-h <NUMBER>: set height\n");
	printf("\t-W: add walls\n");
	printf("\t-H: print usage\n");
}

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
	
	w = newwin(opts.height + 2, opts.width + 2, starty - 1, startx - 1);

	SnakeGame *sg = AllocSnakeGame(opts.width, opts.height, opts.addWalls);

	Matrix *m = sg->field;
	int stop = 0;
	double timeToReset;
	int gameOver = 0;
	long prevTimestamp = timeMs();
	long dt;
	long blinkTimer;

	while (!stop) {
		dt = timeMs() - prevTimestamp;
		prevTimestamp = timeMs();
		blinkTimer = fmod(blinkTimer + dt, 2 * DEATH_BLINK_FREQ);
		if (sg->state == GAME_OVER) {
			if (!gameOver) {
				gameOver = 1;	
				timeToReset = TIME_TO_RESET;
			}
			timeToReset -= dt;
			if (timeToReset <= 0) {
				SnakeGameReset(sg);
				gameOver = 0;
			}
		}
	        int isSnakeBlinking = gameOver && blinkTimer > DEATH_BLINK_FREQ;
		for (int x = 0; x < m->w; x++) {
			for (int y = 0; y < m->h; y++) {
				SnakeGameObject *o = MatrixGet(m, x, y);
				char c;
				switch (*o) {
				case SNAKE:
					c = isSnakeBlinking ? ' ' : 'x';
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
				mvwaddch(w, y + 1, x + 1, c);
			}
		}
		
		// TODO: fix displaying borders
		box(w, 0, 0);

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

	while ((c = getopt(argc, argv, "HWw:h:")) != -1) {
		switch (c) {
		case 'W':
			opts->addWalls = 1;
			break;
		case 'w':
			int w = strtol(optarg, NULL, 10);
			if (errno == EINVAL) {
				fprintf(stderr, "width should be integer");
				endwin();
				exit(-1);
			}

			if (w <= 0) {
				fprintf(stderr, "width should be more than zero");
				endwin();
				exit(-1);
			}

			if (w > sysInfo.maxWidth) {
				fprintf(stderr, "width is more than maximum terminal screen width");
				endwin();
				exit(-1);
			}

			opts->width = w;
			
			break;
		case 'h':
			int h = strtol(optarg, NULL, 10);
			if (errno == EINVAL) {
				fprintf(stderr, "height should be integer");
				endwin();
				exit(-1);
			}

			if (h <= 0) {
				fprintf(stderr, "height should be more than zero");
				endwin();
				exit(-1);
			}

			if (h > sysInfo.maxWidth) {
				fprintf(stderr, "height is more than maximum terminal screen height");
				endwin();
				exit(-1);
			}

			opts->height = h;

			break;

		case 'H':
			endwin();
			printUsage();
			exit(0);
			break;
		case '?':
			if (optopt == 'w' || optopt == 'h') {
				fprintf(stderr, "Options -w and -h requires arguments");
				endwin();
				exit(-1);
			}
			if (isprint(optopt)) {
				fprintf(stderr, "Unknown option -%c", optopt);
				endwin();
				exit(-1);
			} else {
				fprintf(stderr, "Illegal option character");
				endwin();
				exit(-1);
			}
		default:
			abort();
		}
	}
}
