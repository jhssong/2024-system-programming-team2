#ifndef PERSONAL_TIME_TABLE_H
#define PERSONAL_TIME_TABLE_H

#include "common.h"
#include "util.h"
#include "client.h"

void initialize_screen(void);
void load_user_table(Personal_Table loaded_user_table);
void load_schedule(char* team_name, char* username);
void send_schedule_to_server(void);
void periodic_save(int signum);
void process_input(void);
void draw_table(int cursor_row, int cursor_col);
void update_cell(int cursor_row, int cursor_col);

#endif