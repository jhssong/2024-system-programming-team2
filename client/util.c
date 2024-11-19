#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <termios.h>
#include <fcntl.h>
#include <curses.h>
#include <string.h>

// #define NUMBER_OF_CHAR_AT_A_TIME 1
#define TITLE_START_LINE 3

// void tty_mode(int how) {
// 	static struct termios orig_mode;

// 	if (how == 0) 					// Save current setting
// 		tcgetattr(0, &orig_mode); 
// 	else if (how == 1)				// Recover saved setting
// 		tcsetattr(0, TCSANOW, &orig_mode);
// }

// void set_cr_noecho_mode() {
// 	struct termios ttyinfo;
	
// 	if (tcgetattr(0, &ttyinfo) == -1) {
// 		perror("tcgetattr");
// 		exit(EXIT_FAILURE);
// 	}

// 	ttyinfo.c_lflag &= ~ICANON;		// Set non-canonical mode
// 	ttyinfo.c_lflag &= ~ECHO;		// Disable ECHO bit
// 	ttyinfo.c_cc[VMIN] = NUMBER_OF_CHAR_AT_A_TIME;

// 	if (tcsetattr(0, TCSANOW, &ttyinfo) == -1) {
// 		perror("tcsetattr");
// 		exit(EXIT_FAILURE);
// 	}
// }

// void set_non_blocking_mode() {
//     int terflags;
//     terflags = fcntl(0, F_GETFL);
//     terflags |= O_NONBLOCK;
//     fcntl(0, F_SETFL, terflags);
// }

void show_title(const char* title) {
	move(TITLE_START_LINE, 0);
	for(int i = 0; i < strlen(title) + 4; i++)
        printw("=");
    printw("\n");
    printw("  %s\n", title);
	for(int i = 0; i < strlen(title) + 4; i++)
        printw("=");
	printw("\n\n");
}

int current_line = 0;
int item_start_line = 0;
int item_end_line = 0;

int get_valid_input() {
    int c;
	while((c = getchar()) != EOF && c != 65 && c != 66 && c != 13)
        continue;
    return c;
}

void clear_cursor() {
	for (int i = item_start_line; i <= item_end_line; i++) {
		move(i, 0);
		addstr(" ");
	}
	refresh();
}

void move_cursor(int dir) {  								// dir: 0(up), 1(down)
	clear_cursor();

	if (dir == 0) {
		if (current_line == item_start_line)
			current_line = item_end_line;
		else
			current_line -= 1;
	}
	else if (dir == 1) {
		if (current_line == item_end_line)
			current_line = item_start_line;
		else
			current_line += 1;
	}
	move(current_line, 0);
	addstr(">");
	refresh();
}

int select_item(const char* item_list[], int list_size) {
	int trash_x_value;
	getyx(stdscr, current_line, trash_x_value);				// Get current line value
	(void)(trash_x_value);

	item_start_line = current_line;
	item_end_line = current_line + list_size - 1;

	for (int i = item_start_line, item_index = 0; i <= item_end_line; i++, item_index++) {
		printw("  %d. %s\n", item_index + 1, item_list[item_index]);
	}
	refresh();

	move(item_start_line, 0);
	addstr(">");
	refresh();

	int selection_command;

	while((selection_command = get_valid_input())) {
		switch(selection_command) {
			case 65:										// key up: move up
				move_cursor(0);
				break;
			case 66:										// key down: move down
				move_cursor(1);
				break;
			case 13:										// enter: select menu
				return current_line - item_start_line;
				break;
		}
	}

	return -1;
}