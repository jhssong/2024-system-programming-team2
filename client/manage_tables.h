#ifndef MANAGE_TABLES_H
#define MANAGE_TABLES_H

#include "common.h"
#include "util.h"
#include "client.h"
#include "team_table.h"

void initialize_screen(void);
void send_schedule_to_server(void);
void periodic_save(int signum);
void process_input(void);
void draw_table(int cursor_row, int cursor_col);
void update_cell(int cursor_row, int cursor_col);

#endif