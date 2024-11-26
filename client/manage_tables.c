#include "manage_tables.h"

// Initialize screen for personal table
void initialize_screen(void) {
#ifdef DEBUG_PERSONAL
    initscr();
    curs_set(0);
#endif
    cbreak();             // Disable line buffering
    noecho();             // echo off
    keypad(stdscr, TRUE); // Enable special key input


    start_color();  // Initialize color mode
    //Define color pairs
    init_pair(1, COLOR_WHITE, COLOR_BLACK);  // White text - black background (represents 0)
    init_pair(2, COLOR_BLACK, COLOR_WHITE);  // Black text - white background (represents 1)
    init_pair(3, COLOR_BLACK, COLOR_YELLOW); // Black text - yellow background (cursor position)
}

// send user's schedule to server
void send_schedule_to_server(void) {
    Server_response req_data;
    memset(&req_data, 0, sizeof(Server_response));
    req_data.personal_table = user_table;
    connect_to_server(3, req_data);
}

// call send_schedule_to_server() function every 10 seconds
void periodic_send(int signum) {
    send_schedule_to_server();
    alarm(10);
}

// processing inputs from user
void process_input(void) {
    int cursor_row = 0, cursor_col = 0;

    draw_table(cursor_row, cursor_col);

    int ch;
    while ((ch = getch()) != 'q') {  // end program when pressing 'q'(temporary)
        switch (ch) {
            case KEY_UP:
                if (cursor_row > 0) cursor_row--;
                break;
            case KEY_DOWN:
                if (cursor_row < TABLE_MAX_TIME - 1) cursor_row++;
                break;
            case KEY_LEFT:
                if (cursor_col > 0) cursor_col--;
                break;
            case KEY_RIGHT:
                if (cursor_col < TABLE_MAX_DAY - 1) cursor_col++;
                break;
            case '\n': 
            case ' ':
                update_cell(cursor_row, cursor_col);
                break;
        }

        draw_table(cursor_row, cursor_col);
    }
}

void draw_table(int cursor_row, int cursor_col) {
    clear();

    for (int i = 0; i < TABLE_MAX_TIME; i++) {
        for (int j = 0; j < TABLE_MAX_DAY; j++) {
            if (i == cursor_row && j == cursor_col) {
                attron(COLOR_PAIR(3));  // cursor position: Black text - yellow background
            } else if (user_table[i][j] == 1) {
                attron(COLOR_PAIR(2));  // 1: Black text - white background
            } else {
                attron(COLOR_PAIR(1));  // 0: White text - black background
            }
            
            // draw from (PERSONAL_TABLE_START_ROW, PERSONAL_TABLE_START_COL)
            mvprintw(PERSONAL_TABLE_START_ROW + i, PERSONAL_TABLE_START_COL + j * 4, " %c ", user_table.schedule[i][j] ? 'O' : 'X');
            
            attroff(COLOR_PAIR(1));
            attroff(COLOR_PAIR(2));
            attroff(COLOR_PAIR(3));
        }
    }
    refresh();
}

void update_cell(int cursor_row, int cursor_col) {
    user_table[cursor_row][cursor_col] = !user_table[cursor_row][cursor_col];
}

void personal_main(char* team_name, char* username){
    initialize_screen();
    load_schedule(team_name, username);

    signal(SIGALRM, periodic_send);
    alarm(10);

    process_input();

    send_schedule_to_server();  //send changes before shutting down program
}

#ifdef DEBUG_PERSONAL
int main() {
    initialize_screen();

    load_schedule(schedule);

    signal(SIGALRM, periodic_save);
    alarm(10);

    process_input(schedule);
    endwin();

    save_schedule(schedule);

    return 0;
}
#endif
