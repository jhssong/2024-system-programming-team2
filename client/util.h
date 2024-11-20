#ifndef UTIL_H
#define UTIL_H

#include "common.h"

#define TITLE_START_LINE 3

extern int window_width;
extern int window_height;

void get_window_size();
int check_window_size();
void display_title_bar();
void show_title(const char* title);
int select_menu_item(const char* item_list[], int list_size);

#endif
