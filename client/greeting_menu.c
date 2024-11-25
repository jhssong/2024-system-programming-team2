#include "greeting_menu.h"

int display_greeting_menus() {
	const char* GREETING_MENU_TITLE = "Welcome to the TTT(Team Time Table) Application!";
	const char* menu_list[MENU_SIZE] = {"Enter existing team", "Create new team", "About", "Quit"};

	clear();
	display_title_bar();
	show_title(GREETING_MENU_TITLE);

	int res = select_menu_item(menu_list, MENU_SIZE);

	return res;
}