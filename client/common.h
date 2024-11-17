#ifndef COMMON_H
#include <sys/ioctl.h>

#define COMMON_H

#define APP_NAME "TTT"
#define APP_VERSION "v0.0.1"

extern int window_width;
extern int window_height;

typedef struct {
	char team_list[8][256];
}Team_list;

typedef struct {
	int team_id;
	char team_pw[9];
	char team_name[256];
	int created_time;
}Team_detail;

typedef struct {
	char username[256];
	short schedule[26][7];
}Personal_Table;

typedef struct{
	short team_table[26][7];
}Team_table;

typedef union {
	Team_list team_list;
	Team_detail team_detail;
	Personal_Table personal_table;
	Team_table team_table;
}get_server_response;

void get_window_size();

#endif
