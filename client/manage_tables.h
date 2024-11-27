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
void print_best_times(void);
void draw_table(void);
void update_cell(void);
void table_main(void);

#endif