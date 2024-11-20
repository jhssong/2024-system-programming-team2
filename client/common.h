#ifndef COMMON_H
#define COMMON_H

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <unistd.h>
#include <curses.h>
#include <termios.h>
#include <fcntl.h>
#include <dirent.h>
#include <signal.h>
#include <sys/ioctl.h>

#define APP_NAME "TTT"
#define APP_VERSION "v0.0.1"

#define TERMINAL_MIN_WIDTH 50
#define TERMINAL_MIN_HEIGHT 30 			// 09:00 ~ 22:00

#define TEAM_TABLE_START_ROW 4
#define TEAM_TABLE_START_COL 2
#define PERSONAL_TABLE_START_ROW 4
#define PERSONAL_TABLE_START_COL 19
#define MENU_START_ROW 4
#define MENU_START_COL 36

typedef struct {
	char team_list[8][256];
} Team_list;

typedef struct {
	int team_id;
	char team_pw[9];
	char team_name[256];
	int created_time;
} Team_detail;

typedef struct {
	char username[256];
	short schedule[26][7];
} Personal_Table;

typedef struct{
	short team_table[26][7];
} Team_table;

typedef union {
	Team_list team_list;
	Team_detail team_detail;
	Personal_Table personal_table;
	Team_table team_table;
} Server_response;

#endif
