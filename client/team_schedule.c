#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <dirent.h>

#define DAYS 7
#define TIME 48
#define MAX_TEAM_MEMBERS 8
#define BASE_DIRECTORY "./users/"  // 같은 디렉토리에 위치한 users 디렉토리 안에 있는 개인 스케쥴 배열을 읽음

void load_personal_schedules(int schedules[MAX_TEAM_MEMBERS][TIME][DAYS], int *member_count);
void count_available_time(int schedules[MAX_TEAM_MEMBERS][TIME][DAYS], int member_count, int team_schedule[TIME][DAYS]);
void print_team_schedule(int team_schedule[TIME][DAYS]);

int main() {
    int team_schedule[TIME][DAYS] = {0};
    int member_count = 0;
    int personal_schedules[MAX_TEAM_MEMBERS][TIME][DAYS] = {0};

    load_personal_schedules(personal_schedules, &member_count);
    count_available_time(personal_schedules, member_count, team_schedule);
    print_team_schedule(team_schedule);

    return 0;
}

// 개인 스케쥴 데이터를 읽기
void load_personal_schedules(int schedules[MAX_TEAM_MEMBERS][TIME][DAYS], int *member_count) {
    DIR *dir;
    struct dirent *entry;
    *member_count = 0;

    if ((dir = opendir(BASE_DIRECTORY)) == NULL) {
        perror("디렉토리를 열 수 없습니다");
        exit(EXIT_FAILURE);
    }

    while ((entry = readdir(dir)) != NULL && *member_count < MAX_TEAM_MEMBERS) {
        if (entry->d_type == DT_REG) {
            char filepath[512];
            snprintf(filepath, sizeof(filepath), "%s%s", BASE_DIRECTORY, entry->d_name);

            FILE *file = fopen(filepath, "r");
            if (file == NULL) {
                perror("파일을 열 수 없습니다");
                continue;
            }

            for (int t = 0; t < TIME; t++) {
                for (int d = 0; d < DAYS; d++) {
                    fscanf(file, "%d", &schedules[*member_count][t][d]);
                }
            }
            fclose(file);
            (*member_count)++;
        }
    }
    closedir(dir);
}

//각 유저들의 가능한 시간대를 count함
void count_available_time(int schedules[MAX_TEAM_MEMBERS][TIME][DAYS], int member_count, int team_schedule[TIME][DAYS]) {
    for(int i=0; i<TIME; i++)
    {
        for (int j=0; j<DAYS; j++)
        {
            team_schedule[i][j]=0;
        }
    }

    for (int i = 0; i < TIME; i++) {
        for (int j = 0; j < DAYS; j++) {
            int count = 0;
            for (int k = 0; k < member_count; k++) {
                if (schedules[k][i][j] == 1) {
                    count++;
                }
            }
            team_schedule[i][j] = count;
        }
    }
}
//스케쥴표 출력
void print_team_schedule(int team_schedule[TIME][DAYS]) {
    printf(" Sun Mon Tue Wed Thu Fri Sat ");
    printf("\n");

    for (int i = 0; i < TIME; i++) {
        int hour = i / 2;
        char half;
        if (i % 2 == 0) 
            half = 'A';
        else 
            half = 'B';
        printf("%02d%c  ", hour, half);

        for (int j = 0; j < DAYS; j++) {
            int count = team_schedule[i][j];
            if (count == 0) {
                printf(" X ");
            } else if (count == 1) {
                printf("\033[38;5;159m O \033[0m");  
            } else if (count == 2) {
                printf("\033[38;5;153m O \033[0m");  
            } else if (count == 3) {
                printf("\033[38;5;117m O \033[0m");  
            } else if (count == 4) {
                printf("\033[38;5;111m O \033[0m");  
            } else if (count == 5) {
                printf("\033[38;5;75m O \033[0m");   
            } else if (count == 6) {
                printf("\033[38;5;69m O \033[0m");   
            } else if (count == 7) {
                printf("\033[38;5;33m O \033[0m");   
            } else if (count == 8) {
                printf("\033[38;5;18m O \033[0m");   
            }
        }
        printf("\n");
    }
}