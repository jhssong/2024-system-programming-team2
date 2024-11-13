#include <stdio.h>
#include <stdlib.h>
#include <curses.h>
#include "constant.h"
#include "util.h"
#include "greeting_menu.h"
#include "create_new_team.h"

int main() {
	get_window_size();  									// Get window size and save it in constant.c
	initscr();  											// Turns on curses

	int selected_menu = -1;

	while (selected_menu != 3) {
		selected_menu = display_greeting_menus();

		switch (selected_menu) {
			case 0:											// Enter existing team
				break;
			case 1: 										// Create new team
				display_create_new_team();
				break;
			case 2: 										// About
															// TODO Make About page if have time
				break;
			case 3: 										// Quit
				move(window_height - 1, 0);
				addstr("Press any key to exit...");
				refresh();
				getchar();									// TODO Only for debug remove before publish
				endwin();									// Turn off curses
				return 0;
		}
	}
	
}
