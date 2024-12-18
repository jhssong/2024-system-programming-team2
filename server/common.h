#ifndef COMMON_H
#define COMMON_H

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <time.h>
#include <dirent.h>

#define TABLE_MAX_TIME 26
#define TABLE_MAX_DAY 7

#define TEAM_BASE_DIR "team"
#define TEAM_CONFIG_FILE_NAME "config.txt"
#define USER_CONFIG_FILE_NAME "config.txt"

#define TEAM_FOLDER_MODE 0777
#define TEAM_CONFIG_FILE_MODE 0644
#define USER_FOLDER_MODE 0777
#define USER_CONFIG_FILE_MODE 0644

#define MAX_TEAM_FOLDER_PATH 100
#define MAX_TEAM_CONFIG_FILE_PATH 120
#define MAX_USER_FOLDER_PATH 180
#define MAX_USER_CONFIG_FILE_PATH 200

#define MAX_TEAM_COUNT 8
#define MAX_NAME_SIZE 51
#define MAX_PW_SIZE 9
#define MAX_NAME_SIZE 51
#define CREATED_TIME_LEN 20

#define USER_FOLDER_MODE 0777
#define USER_CONFIG_FILE_MODE 0644
#define USER_SCHEDULE_FILE_MODE 0644

#define MAX_USER_SCHEDULE_FILE_PATH 120

// ----

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

typedef union {
	short user_table[TABLE_MAX_TIME][TABLE_MAX_DAY];
	short team_table[TABLE_MAX_TIME][TABLE_MAX_DAY];
	teamlist team_list;
} response;

typedef struct {
	int status_code;
	char msg[51];
	response res;
} response_packet;

#endif