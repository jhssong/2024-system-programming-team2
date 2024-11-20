#include "menu_selector.h"

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
