#include "util.h"

int window_width = 0;
int window_height = 0;

void get_window_size() {
	struct winsize wbuf;

	if (ioctl(0, TIOCGWINSZ, &wbuf) != -1) {
		window_width = wbuf.ws_col;
		window_height = wbuf.ws_row;
	}
}

int check_window_size() {
	get_window_size();
	
	if (window_width < TERMINAL_MIN_WIDTH) return -1;
	if (window_height < TERMINAL_MIN_HEIGHT) return -1;
	return 0;
}

void display_title_bar() {
	// Show current date on the left
	move(0, 0);
	addstr("2024-12-12");

	// Show app name on the middle
	move(0, window_width / 2 - 2);
	addstr(APP_NAME);

	// Show app version on the right
	move(0, window_width - 6);
	addstr(APP_VERSION);

	// Add divider
	for (int i = 0; i < window_width; i++)
		addstr("-");
	
	refresh();
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
