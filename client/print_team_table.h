#ifndef TEAM_TABLE_H
#define TEAM_TABLE_H

#include <ncurses.h>

#define TABLE_MAX_TIME 26 
#define TABLE_MAX_DAY 7   

#define TEAM_TABLE_START_ROW 4
#define TEAM_TABLE_START_COL 2

void print_team_table(short team_table[TABLE_MAX_TIME][TABLE_MAX_DAY]);

#endif