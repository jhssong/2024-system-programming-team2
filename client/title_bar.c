#include <stdio.h>
#include <curses.h>
#include "constant.h"

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
