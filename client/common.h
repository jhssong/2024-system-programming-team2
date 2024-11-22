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
#define TERMINAL_MIN_HEIGHT 30	// 09:00 ~ 22:00

#define TEAM_TABLE_START_ROW 4		// TODO remove
#define TEAM_TABLE_START_COL 2		// TODO remove
#define PERSONAL_TABLE_START_ROW 4	// TODO remove
#define PERSONAL_TABLE_START_COL 19	// TODO remove
#define MENU_START_ROW 4			// TODO remove
#define MENU_START_COL 36			// TODO remove

#define MAX_NAME_SIZE 51
#define MAX_PW_SIZE 9
#define CREATED_TIME_LEN 20

#define MAX_TEAM_COUNT 8
#define MAX_TEAM_ID_LEN 9  // TODO remove

typedef struct {
	char team_name[MAX_NAME_SIZE];
	char user_name[MAX_NAME_SIZE];
	char user_pw[MAX_PW_SIZE];
} user_info;

typedef struct {
	char team_name[MAX_NAME_SIZE];
	char team_pw[MAX_PW_SIZE];
	char created_time[CREATED_TIME_LEN];
} team_info;

typedef struct {
	char team_list[MAX_TEAM_COUNT][MAX_NAME_SIZE];
	int size;
} team_list;

typedef struct {
	char user_name[MAX_NAME_SIZE];
	char team_name[MAX_NAME_SIZE];
	short user_table[TABLE_MAX_TIME][TABLE_MAX_DAY];
} update_user_table;

typedef union {
	user_info user_info;
	team_list team_list;
	team_info team_info;
	update_user_table update_user_table;
} request;

extern user_info user_info;
extern team_info team_info;
extern team_list team_list;

// deprecated ---------------------------------------------- 

typedef struct {
	char team_list[MAX_TEAM_COUNT][MAX_NAME_SIZE];
	int size;
} Team_list;

typedef struct {
	char team_id[MAX_TEAM_ID_LEN];
	char team_pw[MAX_PW_SIZE];
	char team_name[MAX_NAME_SIZE];
	char created_time[CREATED_TIME_LEN];
} Team_detail;

typedef struct {
	char username[MAX_NAME_SIZE];
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
