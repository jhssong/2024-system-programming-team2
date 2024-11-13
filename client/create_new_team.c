#include <stdio.h>
#include <stdlib.h>
#include <curses.h>
#include "constant.h"
#include "title_bar.h"

#define QUESTION_START_LINE 3

const char* TEAM_NAME_QUESTION = "What is the team name? (max 256 characters)";
const char* TEAM_PW_QUESTION = "Enter the password for the team (max 8 characters)";

void display_create_new_team() {
	clear();
	
	display_title_bar();

	move(QUESTION_START_LINE, 0);
	
	/*
		Team Name Question
	 */
	addstr(TEAM_NAME_QUESTION);
	move(QUESTION_START_LINE + 1, 0);
	addstr(": ");
	refresh();
	
	cbreak();
    echo();
	char team_name[257];
	getstr(team_name);

	// TODO Add valid check
	
	/*
		Team PW Question
	 */
	noecho();
	addstr(TEAM_PW_QUESTION);
	move(QUESTION_START_LINE + 3, 0);
	addstr(": ");
	refresh();

	char team_pw[9];
	getstr(team_pw);

	// TODO Add valid check


	// getchar();
	// tty_mode(0);
	
}