#include <stdio.h>
#include <curses.h>
#include "title_bar.h"
#include "constant.h"

int main() {
	get_window_size();

	initscr();  // turns on curses
	clear(); 	// clear screen

	display_title_bar(window_width);
	move(10, 20);
	addstr("Welcome!");

	refresh();	// update the screen
	getch();	// wait for user input 
	endwin();	// turn off curses

	return 0;
}
