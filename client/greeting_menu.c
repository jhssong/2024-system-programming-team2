#include <stdio.h>
#include <stdlib.h>
#include <curses.h>
#include "constant.h"
#include "title_bar.h"
#include "util.h"

#define START_LINE 3
#define MENU_COUNT 4

const char* GREETING_MENU_TITLE = "Welcome to the TTT(Team Time Table) Application!";
const char* menu_str[MENU_COUNT] = {"Enter existing team", "Create new team", "About", "Quit"};

int current_line = 0;
int menu_start_line = 0;
int menu_end_line = 0;

int get_valid_input() {
    int c;
	while((c = getchar()) != EOF && c != 65 && c != 66 && c != 13) {
        continue;
	}
    return c;
}

void clear_menu_cursor() {
	for (int i = menu_start_line; i <= menu_end_line; i++) {
		move(i, 0);
		addstr(" ");
	}
	refresh();
}

void move_menu_cursor(int dir) {  // 0: up, 1: down
	clear_menu_cursor();

	if (dir == 0) {
		if (current_line == menu_start_line)
			current_line = menu_end_line;
		else
			current_line -= 1;
	}
	else if (dir == 1) {
		if (current_line == menu_end_line)
			current_line = menu_start_line;
		else
			current_line += 1;
	}
	move(current_line, 0);
	addstr(">");
	refresh();
}

int display_greeting_menus() {
	clear();
	display_title_bar();
	show_title(START_LINE, GREETING_MENU_TITLE);

	int trash_x_value;
	getyx(stdscr, current_line, trash_x_value);
	(void)(trash_x_value);

	menu_start_line = current_line;
	menu_end_line = current_line + MENU_COUNT - 1;

	int menu_num = 0;

	for (int i = menu_start_line; i <= menu_end_line; i++, menu_num++) {
		printw("  %d. %s\n", menu_num + 1, menu_str[menu_num]);
	}
	refresh();

	move(menu_start_line, 0);
	addstr(">");
	refresh();

	int menu_input;

	while((menu_input = get_valid_input())) {
		switch(menu_input) {
			case 65:										// key up: move up
				move_menu_cursor(0);
				break;
			case 66:										// key down: move down
				move_menu_cursor(1);
				break;
			case 13:										// enter: select menu
				return current_line - menu_start_line;
				break;
		}
	}

	return -1;
}