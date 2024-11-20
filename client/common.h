#ifndef COMMON_H
#define COMMON_H

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <unistd.h>
#include <curses.h>
#include <termios.h>
#include <fcntl.h>
#include <dirent.h>
#include <signal.h>
#include <sys/ioctl.h>

#define APP_NAME "TTT"
#define APP_VERSION "v0.0.1"

#define TERMINAL_MIN_WIDTH 50
#define TERMINAL_MIN_HEIGHT 30 			// 09:00 ~ 22:00

#define TEAM_TABLE_START_ROW 4
#define TEAM_TABLE_START_COL 2
#define PERSONAL_TABLE_START_ROW 4
#define PERSONAL_TABLE_START_COL 19
#define MENU_START_ROW 4
#define MENU_START_COL 36

#endif
