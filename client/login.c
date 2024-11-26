#include "login.h"

void login() {
	clear();
	display_title_bar();
	show_title("Login to the team");

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

	// Request team pw validation
	teaminfo temp_team;
	strncpy(temp_team.team_name, team_info.team_name, sizeof(temp_team.team_name));
	strncpy(temp_team.team_pw, team_pw, sizeof(team_pw));
	
	request team_login_req_data;
	memset(&team_login_req_data, 0, sizeof(teaminfo));
	team_login_req_data.team_info = temp_team;

	request_packet team_login_req = {
		3, team_login_req_data
	};

#ifdef DEBUG
	printw("[DEBUG] Requesting login to the team.\n");
	printw("[DEBUG]     cmd:  %d\n", team_login_req.cmd);
	printw("[DEBUG]     name: %s\n", team_login_req.req.team_info.team_name);
	printw("[DEBUG]     pw:   %s\n", team_login_req.req.team_info.team_pw);
	refresh();
#endif

	response team_login_res = connect_to_server(team_login_req);

	while (strcmp(team_login_res.msg, "Correct") != 0) {
		clear();
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

		strncpy(temp_team.team_pw, team_pw, sizeof(team_pw));
		team_login_req.req.team_info = temp_team;

	#ifdef DEBUG
		printw("[DEBUG] Requesting login to the team.\n");
		printw("[DEBUG]     cmd:  %d\n", team_login_req.cmd);
		printw("[DEBUG]     name: %s\n", team_login_req.req.team_info.team_name);
		printw("[DEBUG]     pw:   %s\n", team_login_req.req.team_info.team_pw);
		refresh();
	#endif

		team_login_res = connect_to_server(team_login_req);
	}	
	memcpy(team_table, team_login_res.team_table, sizeof(team_table));
	strncpy(team_info.team_pw, team_pw, sizeof(team_pw));


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
            user_pw[user_pw_index++] = ch;
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

	request user_login_req_data;
	memset(&user_login_req_data, 0, sizeof(userinfo));
	user_login_req_data.user_info = new_user;

	request_packet user_login_req = {
		4, user_login_req_data
	};

#ifdef DEBUG
	printw("[DEBUG] Requesting login.\n");
	printw("[DEBUG]     cmd:       %d\n", user_login_req.cmd);
	printw("[DEBUG]     team name: %s\n", user_login_req.req.user_info.team_name);
	printw("[DEBUG]     user name: %s\n", user_login_req.req.user_info.user_name);
	printw("[DEBUG]     user pw:   %s\n", user_login_req.req.user_info.user_pw);
	refresh();
#endif

	response user_login_res = connect_to_server(user_login_req);

	while (strcmp(user_login_res.msg, "Correct") != 0) {
		clear();
		// Password incorrect ask again
		addstr("Wrong password! Enter the user password again");
		printw("\n: ");

		cbreak();
		noecho();
		refresh();
		
		user_pw_index = 0;
		while (user_pw_index < 8) {
			int ch = getch();
			if (ch != EOF && ((ch >= 48 && ch <= 57) 	// Recieve only numeric or alphabet
				|| (ch >= 65 && ch <= 90) 
				|| (ch >= 97 && ch <= 122))) {
				user_pw[user_pw_index++] = ch;
				addch('*');
				refresh();
			}
		}
		user_pw[8] = '\0';
		printw("\n");
		echo();
		refresh();

		strncpy(new_user.user_pw, user_pw, sizeof(user_pw));
		user_login_req.req.user_info = new_user;

	#ifdef DEBUG
		printw("[DEBUG] Requesting login.\n");
		printw("[DEBUG]     cmd:       %d\n", user_login_req.cmd);
		printw("[DEBUG]     team name: %s\n", user_login_req.req.user_info.team_name);
		printw("[DEBUG]     user name: %s\n", user_login_req.req.user_info.user_name);
		printw("[DEBUG]     user pw:   %s\n", user_login_req.req.user_info.user_pw);
		refresh();
	#endif

		user_login_res = connect_to_server(user_login_req);
	}

	user_info = new_user;

	memcpy(user_table, user_login_res.user_table, sizeof(user_table));
#ifdef DEBUG
	getchar();
#endif
	// TODO Go to table screen
	table_main();
}