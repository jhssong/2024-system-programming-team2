#include "select_existing_team.h"

const char* SELECT_MENU_TITLE = "Select Existing Team";

int display_select_existing_team() {
	request_packet packet;
	packet.cmd = 0;
	response res = connect_to_server(packet);

	const char* existing_team_list[MAX_TEAM_COUNT + 1];
	for (int i = 0; i < res.team_list.size; i++) {
		existing_team_list[i] = res.team_list.team_list[i];
	}
	existing_team_list[res.team_list.size] = "Back";

	clear();
	display_title_bar();
	show_title(SELECT_MENU_TITLE);

	int item_index = select_menu_item(existing_team_list, res.team_list.size + 1);

	refresh();

	teaminfo new_team;
	strncpy(new_team.team_name, existing_team_list[item_index], sizeof(MAX_NAME_SIZE));
	team_info = new_team;

	refresh();

	if (item_index == res.team_list.size)
		return -1;

	return item_index;
}