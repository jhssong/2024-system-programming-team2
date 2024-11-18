#include <stdio.h>
#include <stdlib.h>
#include <curses.h>
#include "constant.h"
#include "title_bar.h"
#include "util.h"

#define MENU_SIZE 4

int display_greeting_menus() {
	const char* GREETING_MENU_TITLE = "Welcome to the TTT(Team Time Table) Application!";
	const char* menu_str[MENU_SIZE] = {"Enter existing team", "Create new team", "About", "Quit"};

	clear();
	display_title_bar();
	show_title(GREETING_MENU_TITLE);

	int res = select_item(menu_str, MENU_SIZE);

	return res;
}