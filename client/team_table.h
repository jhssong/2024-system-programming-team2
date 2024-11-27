#ifndef TEAM_TABLE_H
#define TEAM_TABLE_H

#include "common.h"

void print_team_table(void);
void get_time_slot(int index, char *timeSlot);
char *get_day(int col);
void print_slots(int slots[TABLE_MAX_TIME][TABLE_MAX_DAY], int *current_row, int *current_col);
void print_best_times(void);

#endif