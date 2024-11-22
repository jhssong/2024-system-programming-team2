#ifndef COMMON_H
#define COMMON_H

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <time.h>
#include <dirent.h>

#define TEAM_BASE_DIR "team"
#define TEAM_CONFIG_FILE_NAME "config.txt"

#define TEAM_FOLDER_MODE 0777
#define TEAM_CONFIG_FILE_MODE 0644

#define MAX_TEAM_FOLDER_PATH 100
#define MAX_TEAM_CONFIG_FILE_PATH 120

#define MAX_TEAM_COUNT 8
#define MAX_TEAM_NAME_LEN 51
#define MAX_TEAM_PW_LEN 9
#define MAX_USER_NAME_LEN 51
#define CREATED_TIME_LEN 11

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