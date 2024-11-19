#ifndef CONSTANT_H
#include <sys/ioctl.h>
#include <unistd.h>

#define CONSTANT_H

#define APP_NAME "TTT"
#define APP_VERSION "v0.0.1"

#define TERMINAL_MIN_WIDTH 50
#define TERMINAL_MIN_HEIGHT 30				// 09:00~22:00

#define TEAM_TABLE_START_ROW 4
#define TEAM_TABLE_START_COL 2
#define PERSONAL_TABLE_START_ROW 4
#define PERSONAL_TABLE_START_COL 19
#define MENU_START_ROW 4
#define MENU_START_COL 36

extern int window_width;
extern int window_height;

void get_window_size();
int check_window_size();

#endif
