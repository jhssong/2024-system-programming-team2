#include <stdio.h>
#include <stdlib.h>
#include <curses.h>
#include "constant.h"
#include "util.h"
#include "greeting_menu.h"
#include "create_new_team.h"

int main() {
	 initscr();            // curses 시작
    cbreak();             // 라인 버퍼링 해제
    echo();               // 입력한 값을 화면에 출력
    // char text[100];

    // printw("Enter text: ");
    // refresh();

    // // fscanf(stdin, "%s", text);  // 표준 입력으로 값 받기
	//  getstr(text);
    // printw("You entered: %s\n", text);
    // refresh();

    // getch();
    // endwin();             // curses 종료
    // return 0;
	get_window_size();  	// Get window size and save it in constant.c
	initscr();  			// Turns on curses

	int selected_menu = display_greeting_menus();

	switch (selected_menu) {
		case 0:				// Enter existing team
			break;
		case 1: 			// Create new team
			display_create_new_team();
			break;
		case 2: 			// About
			break;
        case 3: 			// Quit
			break;
		default:
			fprintf(stderr, "Something went wrong with selecting menus");
			exit(EXIT_FAILURE);
            break;
	}
    
	getchar();				// TODO Only for debug remove before publish
	endwin();				// turn off curses

	return 0;
}
