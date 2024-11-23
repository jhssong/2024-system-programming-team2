#include "create_new_team.h"

const char* CREATE_NEW_TEAM_TITLE = "Create New Team";
const char* TEAM_NAME_QUESTION = "What is the team name? (max 50 characters)";
const char* TEAM_PW_QUESTION = "Enter the password for the team (8 characters, alphabet or numeric only)";

void display_create_new_team() {
	clear();
	display_title_bar();
	show_title(CREATE_NEW_TEAM_TITLE);

	addstr(TEAM_NAME_QUESTION);						// Ask team name
	printw("\n: ");

	cbreak();
    echo();
	refresh();
	
	char team_name[MAX_NAME_SIZE];
	getstr(team_name);

													// TODO Add valid check (Duplictate check)
	
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
	printw("[DEBUG]     size: %zu\n", sizeof(teaminfo));
	refresh();
#endif

	response res = connect_to_server(req);
	team_info = new_team;
}