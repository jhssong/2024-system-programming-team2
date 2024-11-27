#ifndef UTIL_H
#define UTIL_H

#include "common.h"

#define TITLE_START_LINE 3

extern int window_width;
extern int window_height;

void get_window_size();
int check_window_size();
void display_title_bar();
void display_navigation_options_bar(void);
void show_title(const char* title);

#endif
