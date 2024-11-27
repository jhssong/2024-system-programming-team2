#ifndef MANAGE_USER_TABLE_H
#define MANAGE_USER_TABLE_H

#include "util.h"
#include "common.h"
void load_user_table_from_file(userinfo *new_user, short team_table[TABLE_MAX_TIME][TABLE_MAX_DAY]);
char* update_user_table_file(update_user_table *new_user_table);

#endif