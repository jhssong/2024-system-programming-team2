#include "team_table.h"

void print_team_table(void) {

    //print team table
    mvprintw(TEAM_TABLE_START_ROW - 2, TEAM_TABLE_START_COL + 4 + 8, "Team Table");
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

// idx to time slot
void get_time_slot(int index, char *time_slot) {
    int hour = 9 + (index / 2);
    char half = (index % 2 == 0) ? 'A' : 'B';
    sprintf(time_slot, "%02d%c", hour, half);
}

// return day's name string
char *get_day(int col) {
    char *days[] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};
    return days[col];
}

void print_slots(int slots[TABLE_MAX_TIME][TABLE_MAX_DAY], int *current_row, int *current_col) {
    for (int j = 0; j < TABLE_MAX_DAY; j++) {
        int day_has_slots = 0;

        // check if day with having any slots
        for (int i = 0; i < TABLE_MAX_TIME; i++) {
            if (slots[i][j]) {
                day_has_slots = 1;
                break;
            }
        }

        if (day_has_slots) { // only print days with having time slots
            mvprintw((*current_row)++, *current_col, "%s:", get_day(j)); // print day
            int printed = 0;

            for (int i = 0; i < TABLE_MAX_TIME; i++) {
                if (slots[i][j]) {
                    char timeSlot[10];
                    get_time_slot(i, timeSlot);

                    mvprintw(*current_row, *current_col + (printed * (strlen(timeSlot) + 1)), "%s", timeSlot);
                    printed++;

                    if (printed == 5) { // limit prints for 5 slots per line
                        (*current_row)++;
                        printed = 0;
                    }
                }
            }
            if (printed > 0) {
                (*current_row)++;
            }
        }
    }
}

void print_best_times(void) {
    int max_count = -1, second_max_count = -1;
    int max_slots[TABLE_MAX_TIME][TABLE_MAX_DAY] = {0};
    int second_max_slots[TABLE_MAX_TIME][TABLE_MAX_DAY] = {0};

    // find max and second max
    for (int i = 0; i < TABLE_MAX_TIME; i++) {
        for (int j = 0; j < TABLE_MAX_DAY; j++) {
            if (team_table[i][j] > max_count) {
                second_max_count = max_count;
                memcpy(second_max_slots, max_slots, sizeof(max_slots));

                max_count = team_table[i][j];
                memset(max_slots, 0, sizeof(max_slots));
                max_slots[i][j] = 1;
            } else if (team_table[i][j] == max_count) {
                max_slots[i][j] = 1;
            } else if (team_table[i][j] > second_max_count) {
                second_max_count = team_table[i][j];
                memset(second_max_slots, 0, sizeof(second_max_slots));
                second_max_slots[i][j] = 1;
            } else if (team_table[i][j] == second_max_count) {
                second_max_slots[i][j] = 1;
            }
        }
    }

    // display results
    int current_row = BEST_TIME_START_ROW;
    int current_col = BEST_TIME_START_COL;
    if(max_count == 0){
        mvprintw(current_row++, current_col, "No user here!");
        return;
    } else if (max_count == 1) {
        mvprintw(current_row++, current_col, "Only one user here!");
        return;
    } else if (max_count == 2) {
        mvprintw(current_row++, current_col, "2 user can meet at:");
        print_slots(max_slots, &current_row, &current_col);
        return;
    }

    mvprintw(current_row++, current_col, "%d user can meet at:", max_count);
    print_slots(max_slots, &current_row, &current_col);
    current_row++;

    if (second_max_count > 1) {
        mvprintw(current_row++, current_col, "%d user can meet at:", second_max_count);
        print_slots(second_max_slots, &current_row, &current_col);
    }
}