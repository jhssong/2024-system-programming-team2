#include "client.h"

#define PORT 8080
#define BUFFER_SIZE 100

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
                printf(" X   "); // No availability
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


Server_response function_0(int sock) {
    // get team_directory via server response
    Server_response response;
    memset(&response, 0, sizeof(Server_response));

    int bytes_received = 0;

    bytes_received = recv(sock, &response, sizeof(Server_response), 0);
    if(bytes_received<0){
        perror("Fail to receive team_directory\n");
    }

    return response;
}

// connect to 127.0.0.1:8080
void connect_to_server()
{
    int sock;
    struct sockaddr_in serv_addr;
    char msg[BUFFER_SIZE];
    
    sock = socket(PF_INET, SOCK_STREAM, 0);
    if(sock == -1){
        perror("socket() error");
        exit(1);
    }
    
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    serv_addr.sin_port = htons(PORT);
    
    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) == -1) {
        perror("Connection failed");
        close(sock);
        exit(1);
    }

    int check = recv(sock, msg, BUFFER_SIZE, 0);
    msg[check] = '\0';
    if(strcmp(msg, "Server full") == 0){
        perror("Server is full!\nWait until other client disconnect server!");
        close(sock);
        exit(EXIT_FAILURE);
    }else{
        printf("Connected to Server\n");
    }
    

    int func = 0;
    printf("Enter what function to do: ");
    scanf("%d", &func);
    send(sock, &func, sizeof(func), 0);

    int function_received;
    Server_response response;

    recv(sock, &function_received, sizeof(int), 0);

    switch(function_received)
    {
        case 0: // Will Receive Team_list via server_response union
            response = function_0(sock);
            // will place show team_list function
        break;

        case 1: // will receive Team_detail via server_response union

        break;

        case 2: // will receive Personal_table via server_response union

        break;

        case 3: // will receive Team_table via server_response union
            req_packet.cmd = 3; // Command for requesting team table
            send(sock, &req_packet, sizeof(request_packet), 0);

        
            if ((received_bytes = recv(sock, &res_packet, sizeof(response_packet), 0)) <= 0) {
                perror("Failed to receive team table data");
                close(sock);
                exit(EXIT_FAILURE);
            }

            if (res_packet.status_code == 200) {
                printf("Received Team Table:\n");
                printf_team_table(response_packet.res.team_table);
            } else {
                printf("Failed to get team table: %s\n", res_packet.msg);
            }
            break;
            } else {
                printf("Failed to get team table: %s\n", res_packet.msg);
            }
            break;
    }
    
    close(sock); // close socket
}
