#include "team_table.h"

void print_team_table(void) {

    //print team table
    mvprintw(TEAM_TABLE_START_ROW - 2, (TABLE_MAX_TIME + 4)/2, "Team Table");
    mvprintw(TEAM_TABLE_START_ROW - 1, TEAM_TABLE_START_COL + 4, "Sun Mon Tue Wed Thu Fri Sat");

    for (int i = 0; i < TABLE_MAX_TIME; i++) {
        int hour = 9 + (i / 2);          
        char half = (i % 2 == 0) ? 'A' : 'B'; 

        mvprintw(TEAM_TABLE_START_ROW + i, TEAM_TABLE_START_COL, "%02d%c", hour, half);


        for (int j = 0; j < TABLE_MAX_DAY; j++) {
            int count = team_table[i][j];
            int color_pair;

            // Determine color based on the count
            if (count == 0) {
                color_pair = 10; 
            } else if (count == 1) {
                color_pair = 11;
            } else if (count == 2) {
                color_pair = 12;
            } else if (count == 3) {
                color_pair = 13;
            } else if (count == 4) {
                color_pair = 14;
            } else if (count == 5) {
                color_pair = 15;
            } else if (count == 6) {
                color_pair = 16;
            } else if (count == 7) {
                color_pair = 17;
            } else if (count == 8) {
                color_pair = 18;
            }

            attron(COLOR_PAIR(color_pair));

            if (count == 0) {
                mvprintw(TEAM_TABLE_START_ROW + i, TEAM_TABLE_START_COL + 4 + j * 4, " X ");
            } else {
                mvprintw(TEAM_TABLE_START_ROW + i, TEAM_TABLE_START_COL + 4 + j * 4, " %d ", count);
            }

            attroff(COLOR_PAIR(color_pair));
        }
    }
}