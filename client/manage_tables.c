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
    //Define color pairs for user table
    init_pair(1, COLOR_WHITE, COLOR_BLACK);  // White text - black background (represents 0)
    init_pair(2, COLOR_BLACK, COLOR_WHITE);  // Black text - white background (represents 1)
    init_pair(3, COLOR_BLACK, COLOR_YELLOW); // Black text - yellow background (cursor position)

    // define color pairs for team table
    init_pair(11, COLOR_WHITE, COLOR_BLACK);
    init_pair(12, COLOR_RED, COLOR_BLACK);
    init_pair(13, COLOR_YELLOW, COLOR_BLACK);
    init_pair(14, COLOR_GREEN, COLOR_BLACK);
    init_pair(15, COLOR_CYAN, COLOR_BLACK);
    init_pair(16, COLOR_BLUE, COLOR_BLACK);
    init_pair(17, COLOR_MAGENTA, COLOR_BLACK);
}

// send user's schedule to server
void send_schedule_to_server(void) {
    update_user_table new_user_table;
    memset(new_user_table.user_table, 0, sizeof(new_user_table.user_table));

    strncpy(new_user_table.user_name, user_info.user_name, sizeof(new_user_table.user_name));
    strncpy(new_user_table.team_name, team_info.team_name, sizeof(new_user_table.team_name));
    memcpy(new_user_table.user_table, user_table, sizeof(new_user_table.user_table));

    request update_user_table_req_data;
    memset(&update_user_table_req_data, 0, sizeof(update_user_table));
    update_user_table_req_data.user_table = new_user_table;

    request_packet update_user_table_req={
        5, update_user_table_req_data
    };

    response_packet update_user_table_response_packet = connect_to_server(update_user_table_req);
    response update_user_table_res = update_user_table_response_packet.data;

    /*
    if (update_user_table_res.msg != NULL && strcmp(update_user_table_res.msg, "Success") == 0) {
        if (sizeof(update_user_table_res.team_table) == sizeof(team_table)) {
            memcpy(team_table, update_user_table_res.team_table, sizeof(team_table));
        }
    }
    */

    if (strcmp(update_user_table_response_packet.msg, "Success") == 0) {
        if (sizeof(update_user_table_res.team_table) == sizeof(team_table)) {
            memcpy(team_table, update_user_table_res.team_table, sizeof(team_table));
#ifdef DEBUG
            printw("[INFO] Team table successfully updated.\n");
            refresh();
#endif
        } else {
#ifdef DEBUG
            printw("[ERROR] Team table size mismatch. Update aborted.\n");
            refresh();
#endif
        }
    } else {
#ifdef DEBUG
        printw("[ERROR] Unknown server response: %s\n", update_user_table_response_packet.msg);
        refresh();
#endif
    }

    //TODO check res msg and update team table array
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
    display_title_bar();
    print_team_table();
    for (int i = 0; i < TABLE_MAX_TIME; i++) {
        int hour = 9 + (i / 2);          
        char half = (i % 2 == 0) ? 'A' : 'B'; 
        mvprintw(USER_TABLE_START_ROW + i, USER_TABLE_START_COL - 4, "%02d%c", hour, half);
        
        for (int j = 0; j < TABLE_MAX_DAY; j++) {
            if (i == cursor_row && j == cursor_col) {
                attron(COLOR_PAIR(3));  // cursor position: Black text - yellow background
            } else if (user_table[i][j] == 1) {
                attron(COLOR_PAIR(2));  // 1: Black text - white background
            } else {
                attron(COLOR_PAIR(1));  // 0: White text - black background
            }
            
            // draw from (USER_TABLE_START_ROW, USER_TABLE_START_COL)
            mvprintw(USER_TABLE_START_ROW + i, USER_TABLE_START_COL + j * 4, " %c ", user_table[i][j] ? 'O' : 'X');
            
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

void table_main(){
    initialize_screen();

    signal(SIGALRM, periodic_send);

    
    alarm(10);

    process_input();

    alarm(0);   //stop alarm
    send_schedule_to_server();  //send changes before shutting down program
}