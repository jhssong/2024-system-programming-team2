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
#include <sys/stat.h>

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

#define TEAM_BASE_DIR "team"
#define TEAM_CONFIG_FILE_NAME "config.txt"

#define TEAM_FOLDER_MODE 0777
#define TEAM_CONFIG_FILE_MODE 0644

#define MAX_TEAM_FOLDER_PATH 100
#define MAX_TEAM_CONFIG_FILE_PATH 120

#define MAX_TEAM_COUNT 8
#define MAX_TEAM_NAME_LEN 51
#define MAX_TEAM_PW_LEN 9
#define CREATED_TIME_LEN 11
#define MAX_USER_NAME_LEN 51

typedef struct {
	char team_list[MAX_TEAM_COUNT][MAX_TEAM_NAME_LEN];
	int size;
} Team_list;

typedef struct {
	int team_id;
	char team_pw[MAX_TEAM_PW_LEN];
	char team_name[MAX_TEAM_NAME_LEN];
	char created_time[CREATED_TIME_LEN];
} Team_detail;

typedef struct {
	char username[MAX_USER_NAME_LEN];
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
