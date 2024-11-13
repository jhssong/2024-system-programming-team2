#include <stdio.h>
#include <stdlib.h>
#include <curses.h>
#include "constant.h"
#include "title_bar.h"

#define QUESTION_START_LINE 3

const char* TITLE = "Creat New Team";

const char* TEAM_NAME_QUESTION = "What is the team name? (max 256 characters)";
const char* TEAM_PW_QUESTION = "Enter the password for the team (8 characters, alphabet or numeric only)";

void show_title() {
	move(QUESTION_START_LINE, 0);
	printw("====================\n");
	printw("  %s\n", TITLE);
	printw("====================\n");
}

void display_create_new_team() {
	clear();
	display_title_bar();

	show_title();

	// move(QUESTION_START_LINE, 0);
	printw("\n");

	addstr(TEAM_NAME_QUESTION);							// Ask team name
	printw("\n: ");

	cbreak();
    echo();
	refresh();
	
	
	char team_name[257];
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

	// TODO Send data to server	
}