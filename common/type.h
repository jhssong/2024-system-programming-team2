#ifndef TYPE_H
#define TYPE_H

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