#include "team_table.h"

void print_team_table(short team_table[TABLE_MAX_TIME][TABLE_MAX_DAY]) {

    start_color();

    // define color pairs
    init_pair(1, COLOR_WHITE, COLOR_BLACK);
    init_pair(2, COLOR_RED, COLOR_BLACK);
    init_pair(3, COLOR_YELLOW, COLOR_BLACK);
    init_pair(4, COLOR_GREEN, COLOR_BLACK);
    init_pair(5, COLOR_CYAN, COLOR_BLACK);
    init_pair(6, COLOR_BLUE, COLOR_BLACK);
    init_pair(7, COLOR_MAGENTA, COLOR_BLACK);

    //print team table
    mvprintw(TEAM_TABLE_START_ROW - 1, TEAM_TABLE_START_COL + 6, "Sun   Mon   Tue   Wed   Thu   Fri   Sat");

    for (int i = 0; i < TABLE_MAX_TIME; i++) {
        int hour = 9 + (i / 2);          
        char half = (i % 2 == 0) ? 'A' : 'B'; 

        mvprintw(TEAM_TABLE_START_ROW + i, TEAM_TABLE_START_COL, "%02d%c", hour, half);


        for (int j = 0; j < TABLE_MAX_DAY; j++) {
            int count = team_table[i][j];
            int color_pair;

            // Determine color based on the count
            if (count == 0) {
                color_pair = 1; 
            } else if (count == 1) {
                color_pair = 1;
            } else if (count == 2) {
                color_pair = 1;
            } else if (count == 3) {
                color_pair = 2;
            } else if (count == 4) {
                color_pair = 3;
            } else if (count == 5) {
                color_pair = 4;
            } else if (count == 6) {
                color_pair = 5;
            } else if (count == 7) {
                color_pair = 6;
            } else if (count == 8) {
                color_pair = 7;
            }

            attron(COLOR_PAIR(color_pair));

            if (count == 0) {
                mvprintw(TEAM_TABLE_START_ROW + i, TEAM_TABLE_START_COL + 6 + j * 6, " X   ");
            } else {
                mvprintw(TEAM_TABLE_START_ROW + i, TEAM_TABLE_START_COL + 6 + j * 6, " %d   ", count);
            }

            attroff(COLOR_PAIR(color_pair));
        }
    }

    refresh();

    // Wait for user input to exit
    while (1) {
        char ch = getch();
        if (ch == 'q') {
            break;
        }
    }
}