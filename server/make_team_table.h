#ifndef MAKE_TEAM_TABLE_H
#define MAKE_TEAM_TABLE_H


#include "common.h"


#define TABLE_MAX_DAY 7
#define TABLE_MAX_TIME 26
#define MAX_CLIENTS 8
#define TEAMS_DIRECTORY "team/"

void make_team_table(short table[MAX_CLIENTS][TABLE_MAX_TIME][TABLE_MAX_DAY], int *member_count, char *team_name);
void count_available_time(short table[MAX_CLIENTS][TABLE_MAX_TIME][TABLE_MAX_DAY], int member_count);

#endif