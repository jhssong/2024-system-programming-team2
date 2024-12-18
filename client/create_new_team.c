#include "create_new_team.h"

const char* CREATE_NEW_TEAM_TITLE = "Create New Team";
const char* TEAM_NAME_QUESTION = "What is the team name? (max 50 characters)";
const char* TEAM_PW_QUESTION = "Enter the password for the team (8 characters, alphabet or numeric only)";

int display_create_new_team() {
	flushinp();
	clear();
	display_title_bar();
	show_title(CREATE_NEW_TEAM_TITLE);
	
	char team_name[MAX_NAME_SIZE];
	int is_ask_team_name_again = 0;
	while (1) {
		int check_name = 1;
		if (is_ask_team_name_again) addstr("Already existing team name. Type another name");
		else addstr(TEAM_NAME_QUESTION);						// Ask team name
		printw("\n: ");

		cbreak();
		echo();
		refresh();
		
		getstr(team_name);

		request_packet check_team_count_packet;
		check_team_count_packet.cmd = 0;
		
		response_packet check_team_count_response_packet = connect_to_server(check_team_count_packet);
		response check_team_count_res = check_team_count_response_packet.data;

		if (check_team_count_res.team_list.size > 7) {
			return 1;
		}

		for (int i = 0; i < check_team_count_res.team_list.size; i++) {
			if (strcmp(check_team_count_res.team_list.team_list[i], team_name) == 0)
				check_name = 0;
		}
		if (check_name) break;
		else is_ask_team_name_again = 1;
	}
	
	addstr(TEAM_PW_QUESTION);						// Ask team password
	printw("\n: ");

	noecho();
	refresh();
	
	char team_pw[9];
	int team_pw_index = 0;
	while (team_pw_index < 8) {
        int ch = getch();
        if (ch != EOF && ((ch >= 48 && ch <= 57) 	// Recieve only numeric or alphabet
			|| (ch >= 65 && ch <= 90) 
			|| (ch >= 97 && ch <= 122))) {
            team_pw[team_pw_index++] = ch;
            addch('*');
            refresh();
        }
    }
	team_pw[8] = '\0';
	
	printw("\n");
	echo();
    refresh();

	teaminfo new_team;
	strncpy(new_team.team_name, team_name, sizeof(team_name));
	strncpy(new_team.team_pw, team_pw, sizeof(team_pw));

	request req_data;
	memset(&req_data, 0, sizeof(teaminfo));
	req_data.team_info = new_team;

	request_packet req = {
		2, req_data
	};

#ifdef DEBUG
	printw("[DEBUG] Requesting creating new team.\n");
	printw("[DEBUG]     cmd:  %d\n", req.cmd);
	printw("[DEBUG]     name: %s\n", req.req.team_info.team_name);
	printw("[DEBUG]     pw:   %s\n", req.req.team_info.team_pw);
	refresh();
#endif

	connect_to_server(req);
	team_info = new_team;

	return 0;
}