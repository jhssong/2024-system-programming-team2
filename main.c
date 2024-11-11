#include <stdio.h>
#include <stdlib.h>
#include <curses.h>
#include "constant.h"
#include "util.h"
#include "greeting_menu.h"

int main() {
	get_window_size();  	// Get window size and save it in constant.c
	initscr();  			// Turns on curses
	tty_mode(0);			// Save original settings
	set_cr_noecho_mode();	// Set non-blocking mode

	int selected_menu = display_greeting_menus();

	switch (selected_menu) {
		case 0:				// Enter existing team
			break;
		case 1: 			// Create new team
			break;
		case 2: 			// About
			break;
		case 3: 			// Quit
			break;
		default:
			fprintf(stderr, "Something went wrong with selecting menus");
			exit(EXIT_FAILURE);
	}

	getchar();				// TODO Only for debug remove before publish
	tty_mode(1);			// Restore original settings
	endwin();				// turn off curses

	return 0;
}
