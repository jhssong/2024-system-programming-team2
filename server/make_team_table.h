#ifndef MAKE_TEAM_TABLE_H
#define MAKE_TEAM_TABLE_H


#include "common.h"

#ifndef MAX_CLIENTS
#define MAX_CLIENTS 8
#endif

#define TEAMS_DIRECTORY "team/"

void make_team_table(short table[MAX_CLIENTS][TABLE_MAX_TIME][TABLE_MAX_DAY], int *member_count, char *team_name);
void count_available_time(short table[MAX_CLIENTS][TABLE_MAX_TIME][TABLE_MAX_DAY], int member_count, short team_table[TABLE_MAX_TIME][TABLE_MAX_DAY]);

#endif