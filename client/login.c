#include "login.h"

void login() {
	// Ask team password
	addstr("Enter the team password");
	printw("\n: ");

	cbreak();
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

	while (strcmp(team_pw, team_info.team_pw) != 0) {
		// Password incorrect ask again
		addstr("Wrong password! Enter the team password again");
		printw("\n: ");

		cbreak();
		noecho();
		refresh();
		
		team_pw_index = 0;
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
	}

	// Ask user name
	addstr("Enter the user name");
	printw("\n: ");

	cbreak();
	refresh();
	
	char user_name[MAX_NAME_SIZE];
	getstr(user_name);

	// Ask user password
	addstr("Enter the user password");
	printw("\n: ");

	cbreak();
    noecho();
	refresh();

	char user_pw[9];
	int user_pw_index = 0;
	while (user_pw_index < 8) {
        int ch = getch();
        if (ch != EOF && ((ch >= 48 && ch <= 57) 	// Recieve only numeric or alphabet
			|| (ch >= 65 && ch <= 90) 
			|| (ch >= 97 && ch <= 122))) {
            team_pw[user_pw_index++] = ch;
            addch('*');
            refresh();
        }
    }
	user_pw[8] = '\0';
	printw("\n");
	echo();
	refresh();

	// Login to the team
	userinfo new_user;
	strncpy(new_user.team_name, team_info.team_name, sizeof(team_info.team_name));
	strncpy(new_user.user_name, user_name, sizeof(user_name));
	strncpy(new_user.user_pw, user_pw, sizeof(user_pw));

	request req_data;
	memset(&req_data, 0, sizeof(userinfo));
	req_data.user_info = new_user;

	request_packet req = {
		4, req_data
	};

#ifdef DEBUG
	printw("[DEBUG] Requesting login.\n");
	printw("[DEBUG]     cmd:       %d\n", req.cmd);
	printw("[DEBUG]     team name: %s\n", req.req.user_info.team_name);
	printw("[DEBUG]     user name: %s\n", req.req.user_info.user_name);
	printw("[DEBUG]     user pw:   %s\n", req.req.user_info.user_pw);
	printw("[DEBUG]     size:      %zu\n", sizeof(userinfo));
	refresh();
#endif

	response res = connect_to_server(req);
	// user_info = new_user;

#ifdef DEBUG
	getchar();
#endif
}