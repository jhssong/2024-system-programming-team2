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

#define TABLE_MAX_TIME 26
#define TABLE_MAX_DAY 7

#define TEAM_BASE_DIR "team"
#define TEAM_CONFIG_FILE_NAME "config.txt"

#define TEAM_FOLDER_MODE 0777
#define TEAM_CONFIG_FILE_MODE 0644

#define MAX_TEAM_FOLDER_PATH 100
#define MAX_TEAM_CONFIG_FILE_PATH 120

#define MAX_TEAM_COUNT 8
#define MAX_TEAM_ID_LEN 9
#define MAX_TEAM_NAME_LEN 51
#define MAX_TEAM_PW_LEN 9
#define MAX_USER_NAME_LEN 51
#define CREATED_TIME_LEN 20

#define USER_FOLDER_MODE 0777
#define USER_CONFIG_FILE_MODE 0644
#define USER_SCHEDULE_FILE_MODE 0644

#define MAX_USER_FOLDER_PATH 100
#define MAX_USER_SCHEDULE_FILE_PATH 120
#define MAX_USER_CONFIG_FILE_PATH 120

typedef struct {
	char team_list[MAX_TEAM_COUNT][MAX_TEAM_NAME_LEN];
} Team_list;

typedef struct {
	char team_id[MAX_TEAM_ID_LEN];
	char team_pw[MAX_TEAM_PW_LEN];
	char team_name[MAX_TEAM_NAME_LEN];
	char created_time[CREATED_TIME_LEN];
} Team_detail;

typedef struct {
	char team_name[MAX_TEAM_NAME_LEN];
	char username[MAX_USER_NAME_LEN];
	short schedule[TABLE_MAX_TIME][TABLE_MAX_DAY];
} Personal_Table;

typedef struct{
	char team_name[MAX_TEAM_NAME_LEN];
	short team_table[TABLE_MAX_TIME][TABLE_MAX_DAY];
} Team_table;

typedef union {
	Team_list team_list;
	Team_detail team_detail;
	Personal_Table personal_table;
	Team_table team_table;
} Server_response;

#endif
