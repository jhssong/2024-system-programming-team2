#include "util.h"
#include "client.h"
#include "common.h"
#include "greeting_menu.h"
#include "create_new_team.h"
#include "select_existing_team.h"
#include "personal_time_table.h"
#include "login.h"

void handle_quit(int signum) {
	clear();
	move(window_height - 1, 0);
	addstr("Press any key to exit...");
	refresh();
	getchar();									// TODO Only for debug remove before publish
	endwin();									// Turn off curses
	exit(0);
}

int main() {
	signal(SIGINT, handle_quit);
	init_global_variable();
	get_window_size();  									// Get window size and save it in constant.c
	initscr();  											// Turns on curses
	curs_set(0);

	int valid_window_size = check_window_size();			//  Check valid terminal size
	while (valid_window_size == -1) {
		clear();
		move(0, 0);
		printw("Minimum terminal size is %d x %d, current: %d x %d", 
			TERMINAL_MIN_WIDTH, TERMINAL_MIN_HEIGHT, window_width, window_height);
		refresh();
		sleep(1);
		valid_window_size =  check_window_size();
	}
	
	int selected_menu = -1;

	while (selected_menu != 3) {
		selected_menu = display_greeting_menus();

		switch (selected_menu) {
			case 0:											// Enter existing team
				int res = display_select_existing_team();
				if (res == -1) break;
				login();
				break;
			case 1: 										// Create new team
				display_create_new_team();
				login();
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
