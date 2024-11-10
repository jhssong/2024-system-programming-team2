#include <stdio.h>
#include <curses.h>

int main() {
	initscr();  // turns on curses
	clear(); 	// clear screen
	move(10, 20);
	addstr("Welcome!");

	refresh();	// update the screen
	getch();	// wait for user input 
	endwin();	// turn off curses

	return 0;
}
