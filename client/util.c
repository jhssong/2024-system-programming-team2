#include "util.h"

int window_width = 0;
int window_height = 0;

void get_window_size() {
	struct winsize wbuf;

	if (ioctl(0, TIOCGWINSZ, &wbuf) != -1) {
		window_width = wbuf.ws_col;
		window_height = wbuf.ws_row;
	}
}

int check_window_size() {
	get_window_size();
	
	if (window_width < TERMINAL_MIN_WIDTH) return -1;
	if (window_height < TERMINAL_MIN_HEIGHT) return -1;
	return 0;
}

void display_title_bar() {
	// Show current date on the left
	move(0, 0);
	addstr("2024-12-12");

	// Show app name on the middle
	move(0, window_width / 2 - 2);
	addstr(APP_NAME);

	// Show app version on the right
	move(0, window_width - 6);
	addstr(APP_VERSION);

	// Add divider
	for (int i = 0; i < window_width; i++)
		addstr("-");
	
	refresh();
}

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

void move_menu_cursor(int dir) {  								// dir: 0(up), 1(down)
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

int select_menu_item(const char* item_list[], int list_size) {
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
				move_menu_cursor(0);
				break;
			case 66:										// key down: move down
				move_menu_cursor(1);
				break;
			case 13:										// enter: select menu
				return current_line - item_start_line;
				break;
		}
	}
	return -1;
}
