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

#include "util.h"

#define APP_NAME "TTT"
#define APP_VERSION "v0.0.1"

#define TERMINAL_MIN_WIDTH 50
#define TERMINAL_MIN_HEIGHT 30	// 09:00 ~ 22:00

#define USER_TABLE_START_ROW 4
#define USER_TABLE_START_COL 58
#define TEAM_TABLE_START_ROW 4
#define TEAM_TABLE_START_COL 2

#define MAX_NAME_SIZE 51
#define MAX_PW_SIZE 9
#define CREATED_TIME_LEN 20

#define MAX_TEAM_COUNT 8

#define TABLE_MAX_TIME 26
#define TABLE_MAX_DAY 7

typedef struct {
	char team_name[MAX_NAME_SIZE];
	char user_name[MAX_NAME_SIZE];
	char user_pw[MAX_PW_SIZE];
} userinfo;

typedef struct {
	char team_name[MAX_NAME_SIZE];
	char team_pw[MAX_PW_SIZE];
	char created_time[CREATED_TIME_LEN];
} teaminfo;

typedef struct {
	char team_list[MAX_TEAM_COUNT][MAX_NAME_SIZE];
	int size;
} teamlist;

typedef struct {
	char user_name[MAX_NAME_SIZE];
	char team_name[MAX_NAME_SIZE];
	short user_table[TABLE_MAX_TIME][TABLE_MAX_DAY];
} update_user_table;

typedef union {
	userinfo user_info;
	teaminfo team_info;
	update_user_table user_table;
} request;

typedef struct {
	int cmd;
	request req;
} request_packet;

extern userinfo user_info;
extern teaminfo team_info;
extern teamlist team_list;
extern short user_table[TABLE_MAX_TIME][TABLE_MAX_DAY];
extern short team_table[TABLE_MAX_TIME][TABLE_MAX_DAY];

typedef union {
	short user_table[TABLE_MAX_TIME][TABLE_MAX_DAY];
	short team_table[TABLE_MAX_TIME][TABLE_MAX_DAY];
	teamlist team_list;
} response;

typedef struct {
	int status_code;
	char msg[51];
	response data;
} response_packet;

void init_global_variable();

#endif