#include <stdio.h>
#include <stdlib.h>
#include <curses.h>
#include "constant.h"
#include "title_bar.h"

#define MENU_LEFT_PADDING 3		// Should be greater than 1
#define MENU_START_LINE 5		// Should be greater than 3
#define MENU_END_LINE 8			// Check the total menu lines

int current_line = MENU_START_LINE;

int get_valid_input() {
    int c;
	while((c = getchar()) != EOF && c != 65 && c != 66 && c != 13) {
        continue;
	}
    return c;
}

void move_menu_cursor(int dir) {  // 0: up, 1: down
	if (dir == 0) {
		if (current_line == MENU_START_LINE)
			current_line = MENU_END_LINE;
		else
			current_line -= 1;
	}
	else if (dir == 1) {
		if (current_line == MENU_END_LINE)
			current_line = MENU_START_LINE;
		else
			current_line += 1;
	}
	move(current_line, MENU_LEFT_PADDING);
	refresh();
}

int display_greeting_menus() {
	clear();
	display_title_bar();

	const char* menu_str[4] = {"Enter existing team", "Create new team", "About", "Quit"};

	move(MENU_START_LINE - 2, MENU_LEFT_PADDING);
	addstr("Welcome to the TTT(Team Time Table) Application!");

	for (int i = MENU_START_LINE; i <= MENU_END_LINE; i++) {
		move(i, MENU_LEFT_PADDING);
		printw("%d. %s", i - MENU_START_LINE + 1, menu_str[i - MENU_START_LINE]);
	}
	move(current_line, MENU_LEFT_PADDING);
	refresh();

	int menu_input;

	while((menu_input = get_valid_input())) {
		switch(menu_input) {
			case 65:    // key up: move up
				move_menu_cursor(0);
				break;
			case 66:    // key down: move down
				move_menu_cursor(1);
				break;
			case 13:	// enter: select menu
				return current_line - MENU_START_LINE;
				break;
		}
	}

	return -1;
}