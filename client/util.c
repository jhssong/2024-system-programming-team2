#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <termios.h>
#include <fcntl.h>
#include <curses.h>
#include <string.h>
#include <sys/ioctl.h>

#define TITLE_START_LINE 3

int window_width = 0;
int window_height = 0;

void get_window_size() {
	struct winsize wbuf;

	if (ioctl(0, TIOCGWINSZ, &wbuf) != -1) {
		window_width = wbuf.ws_col;
		window_height = wbuf.ws_row;
	}
}

void show_title(const char* title) {
	move(TITLE_START_LINE, 0);
	for(int i = 0; i < strlen(title) + 4; i++)
        printw("=");
    printw("\n");
    printw("  %s\n", title);
	for(int i = 0; i < strlen(title) + 4; i++)
        printw("=");
	printw("\n\n");
}

