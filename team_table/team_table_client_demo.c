#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080
#define BUFFER_SIZE 1024
#define TABLE_MAX_DAY 7
#define TABLE_MAX_TIME 26

short team_table[TABLE_MAX_TIME][TABLE_MAX_DAY] = {0};

void print_team_table(short team_table[TABLE_MAX_TIME][TABLE_MAX_DAY]);

int main() {
    int sock;
    struct sockaddr_in serv_addr;

    sock = socket(PF_INET, SOCK_STREAM, 0);
    if (sock == -1) {
        perror("socket() error");
        exit(1);
    }

    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    serv_addr.sin_port = htons(PORT);

    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) == -1) {
        perror("connect() error");
        close(sock);
        exit(1);
    }

    printf("Connected to server.\n");

    // Request team table
    const char *request = "GET_TEAM_TABLE";
    send(sock, request, strlen(request), 0);

    // Receive team_table data
    int bytes_received = recv(sock, team_table, sizeof(team_table), 0);
    if (bytes_received == sizeof(team_table)) {
        printf("Received team table successfully.\n");
        print_team_table(team_table);  // Print the received table
    } else {
        printf("Error: Incomplete or corrupted data received.\n");
    }

    close(sock);
    return 0;
}

// Print the team table
void print_team_table(short team_table[TABLE_MAX_TIME][TABLE_MAX_DAY]) {
    printf("      Sun   Mon   Tue   Wed   Thu   Fri   Sat\n");

    for (int i = 0; i < TABLE_MAX_TIME; i++) {
        int hour = 9 + (i / 2); 
        char half = (i % 2 == 0) ? 'A' : 'B'; 
        printf("%02d%c    ", hour, half); 

        
        for (int j = 0; j < TABLE_MAX_DAY; j++) {
            int count = team_table[i][j];
            
            // Color and content based on count
            if (count == 0) {
                printf("  X   "); // No availability
            } else if (count == 1) {
                printf("\033[38;5;159m■     \033[0m"); //Lightest blue
            } else if (count == 2) {
                printf("\033[38;5;153m■     \033[0m"); 
            } else if (count == 3) {
                printf("\033[38;5;117m■     \033[0m"); 
            } else if (count == 4) {
                printf("\033[38;5;111m■     \033[0m"); 
            } else if (count == 5) {
                printf("\033[38;5;75m■     \033[0m"); 
            } else if (count == 6) {
                printf("\033[38;5;69m■     \033[0m"); 
            } else if (count == 7) {
                printf("\033[38;5;33m■     \033[0m"); 
            } else if (count == 8) {
                printf("\033[38;5;18m■     \033[0m"); // Darkest blue
            }
        }
        printf("\n");
    }
}