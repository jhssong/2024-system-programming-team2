#include "create_new_team.h"

const char* CREATE_NEW_TEAM_TITLE = "Create New Team";
const char* TEAM_NAME_QUESTION = "What is the team name? (max 50 characters)";
const char* TEAM_PW_QUESTION = "Enter the password for the team (8 characters, alphabet or numeric only)";

void display_create_new_team() {
	clear();
	display_title_bar();
	show_title(CREATE_NEW_TEAM_TITLE);

	addstr(TEAM_NAME_QUESTION);							// Ask team name
	printw("\n: ");

	cbreak();
    echo();
	refresh();	
	
	char team_name[51];
	getstr(team_name);

	// TODO Add valid check (Duplictate check)
	
	addstr(TEAM_PW_QUESTION);							// Ask team password
	printw("\n: ");

	noecho();
	refresh();
	
	char team_pw[9];
	int team_pw_index = 0;
	while (team_pw_index < 8) {
        int ch = getch();
        if (ch != EOF && ((ch >= 48 && ch <= 57) 		// Recieve only numeric or alphabet
			|| (ch >= 65 && ch <= 90) 
			|| (ch >= 97 && ch <= 122))) {
            team_pw[team_pw_index++] = ch;
            addch('*');
            refresh();
        }
    }
	team_pw[8] = '\0';

	echo();
    refresh();

	Team_detail new_team;
	strncpy(new_team.team_name, team_name, sizeof(team_name));
	strncpy(new_team.team_pw, team_pw, sizeof(team_pw));

	// Server_response union에 new_team을 할당
	Server_response req_data;
	memset(&req_data, 0, sizeof(Server_response)); // 초기화
	req_data.team_detail = new_team; // team_detail 멤버에 new_team 할당

	connect_to_server(1, req_data);  // req_data는 Server_response 타입
}