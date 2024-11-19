#include <stdio.h>
#include <stdlib.h>
#include <curses.h>
#include "constant.h"
#include "title_bar.h"
#include "util.h"



int display_select_existing_team() {
	const char* SELECT_MENU_TITLE = "Select Existing Team";
	const char* test_team_list[] = {"Team Good", "TeamTeam", "Simple", "Modak Fire"};
	const test_team_size = sizeof(test_team_list) / sizeof(char*);

	clear();
	display_title_bar();
	show_title(SELECT_MENU_TITLE);

	int item_index = select_item(test_team_list, test_team_size);

	refresh();
	
	getchar();

	return item_index;
}