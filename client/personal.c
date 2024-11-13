#include <curses.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

#define TIME 24             // 시간 단위 (행: 1시간 단위로 24시간)
#define DAYS 7              // 요일 단위 (열: 7일)
#define PERSONAL_X 10       // 개인 표 시작 X좌표
#define PERSONAL_Y 5        // 개인 표 시작 Y좌표
#define FILENAME "schedule.bin"  // 배열을 저장할 바이너리 파일 이름(임시로 지정 - 서버 추가시 [username].bin)

// 함수 선언
void initialize_screen();
void load_schedule(short schedule[TIME][DAYS]);
void save_schedule(const short schedule[TIME][DAYS]);
void draw_table(short schedule[TIME][DAYS], int cursor_row, int cursor_col);
void process_input(short schedule[TIME][DAYS]);
void update_cell(short schedule[TIME][DAYS], int cursor_row, int cursor_col);
void periodic_save(int signum);

short schedule[TIME][DAYS] = {0};

// 화면 초기화 함수
void initialize_screen() {
    initscr();            // curses 모드 시작
    cbreak();             // 입력 문자 단위로 처리
    noecho();             // echo off
    keypad(stdscr, TRUE); // 특수 키 입력 사용 가능하도록 변경
    curs_set(0);          // 커서 숨김

    start_color();
    init_pair(1, COLOR_WHITE, COLOR_BLACK);  // 0인 칸 (배경 검정, 텍스트 하양)
    init_pair(2, COLOR_BLACK, COLOR_WHITE);  // 1인 칸 (배경 하양, 텍스트 검정)
    init_pair(3, COLOR_BLACK, COLOR_YELLOW); // 커서 위치(노랑)
}

// schedule 배열을 바이너리 파일에서 불러옴
void load_schedule(short schedule[TIME][DAYS]) {
    FILE *file = fopen(FILENAME, "rb");
    if (file) {
        fread(schedule, sizeof(short), TIME * DAYS, file);
        fclose(file);
    } else {
        // 파일이 없을 경우 0으로 초기화된 배열을 저장
        save_schedule(schedule);
    }
}

// schedule 배열을 바이너리 파일에 저장
void save_schedule(const short schedule[TIME][DAYS]) {
    FILE *file = fopen(FILENAME, "wb");
    if (file) {
        fwrite(schedule, sizeof(short), TIME * DAYS, file);
        fclose(file);
    } else {
        perror("파일 저장 오류");
    }
}

// 10초마다 호출되어 schedule을 저장(+서버로 파일 전송 추가하기)
void periodic_save(int signum) {
    save_schedule(schedule);
    alarm(10);
}

// 입력 처리 함수
void process_input(short schedule[TIME][DAYS]) {
    int cursor_row = 0, cursor_col = 0;

    draw_table(schedule, cursor_row, cursor_col);

    int ch;
    while ((ch = getch()) != 'q') {  // q 키 입력 시 프로그램 종료(임시)
        switch (ch) {
            case KEY_UP:
                if (cursor_row > 0) cursor_row--;
                break;
            case KEY_DOWN:
                if (cursor_row < TIME - 1) cursor_row++;
                break;
            case KEY_LEFT:
                if (cursor_col > 0) cursor_col--;
                break;
            case KEY_RIGHT:
                if (cursor_col < DAYS - 1) cursor_col++;
                break;
            case '\n':  // 엔터나 스페이스 입력 시 배열 변경
            case ' ':
                update_cell(schedule, cursor_row, cursor_col);
                break;
        }

        draw_table(schedule, cursor_row, cursor_col);  // 테이블 다시 그리기
    }
}

void draw_table(short schedule[TIME][DAYS], int cursor_row, int cursor_col) {
    clear();

    for (int i = 0; i < TIME; i++) {
        for (int j = 0; j < DAYS; j++) {
            if (i == cursor_row && j == cursor_col) {
                attron(COLOR_PAIR(3));  // 선택된 셀 색상 반전
            } else if (schedule[i][j] == 1) {
                attron(COLOR_PAIR(2));  // 1인 칸 흰색
            } else {
                attron(COLOR_PAIR(1));  // 0인 칸 검정색
            }
            
            // (PERSONAL_Y, PERSONAL_X)부터 그리기
            mvprintw(PERSONAL_Y + i, PERSONAL_X + j * 4, " %c ", schedule[i][j] ? 'O' : 'X');
            
            // 색상 속성 off
            attroff(COLOR_PAIR(1));
            attroff(COLOR_PAIR(2));
            attroff(COLOR_PAIR(3));
        }
    }
    refresh();
}

void update_cell(short schedule[TIME][DAYS], int cursor_row, int cursor_col) {
    schedule[cursor_row][cursor_col] = !schedule[cursor_row][cursor_col];
}

// main 함수
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
