#include "select_existing_team.h"

const char* SELECT_MENU_TITLE = "Select Existing Team";

int display_select_existing_team() {
	request_packet packet;
	packet.cmd = 0;
	response res = connect_to_server(packet);

	const char* item_ptrs[MAX_TEAM_COUNT];
	for (int i = 0; i < res.team_list.size; i++) {
		item_ptrs[i] = res.team_list.team_list[i];
	}
	clear();
	display_title_bar();
	show_title(SELECT_MENU_TITLE);

	int item_index = select_menu_item(item_ptrs, res.team_list.size);

	refresh();

	getchar();

	return item_index;
}