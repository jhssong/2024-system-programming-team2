#include "client.h"

response connect_to_server(request_packet req) {
    int sock;
    int received_bytes;
    struct sockaddr_in serv_addr;

    response_packet res;

    sock = socket(PF_INET, SOCK_STREAM, 0);
    if(sock == -1){
        perror("socket() error");
        exit(EXIT_FAILURE);
    }

    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr(SERVER_ADDRESS);
    serv_addr.sin_port = htons(PORT);

    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) == -1) {
        perror("Connection failed");
        close(sock);
        // exit(EXIT_FAILURE);
        return res.res;
    }
    
    received_bytes = recv(sock, &res, sizeof(response_packet), 0);

    if (received_bytes <= 0) {
        mvprintw(window_height - 1, 0, "Failed to receive data\n");
        refresh();
        close(sock);
        exit(EXIT_FAILURE);
    }

    if (res.status_code == 503) {
        mvprintw(window_height - 1, 0, "[ERROR] 503 error: %s\n", res.msg);
        refresh();
        close(sock);
        exit(EXIT_FAILURE);
    } else {
    #ifdef DEBUG
        printw("[DEBUG] status %d with: %s\n", res.status_code, res.msg);
        refresh();
    #endif
    }

    send(sock, &req, sizeof(request_packet), 0);

    received_bytes = recv(sock, &res, sizeof(response_packet), 0);

    if (received_bytes <= 0) {
        mvprintw(window_height - 1, 0, "Failed to receive data\n");
        refresh();
        close(sock);
        exit(EXIT_FAILURE);
    }

    if (res.status_code == 503) {
        mvprintw(window_height - 1, 0, "%s", res.msg);
        refresh();
        close(sock);
    } else {
    #ifdef DEBUG
        printw("[DEBUG] status %d with: %s\n", res.status_code, res.msg);
        refresh();
    #endif
    }
    return res.res;
}

void old_connect_to_server(int function_choose, Server_response req_data) {
    int sock;
    struct sockaddr_in serv_addr;

    int received_bytes;
    char msg[BUFFER_SIZE];
    
    sock = socket(PF_INET, SOCK_STREAM, 0);
    if(sock == -1){
        mvprintw(window_height - 1, 0, "socket() error");
        // perror("socket() error");
        exit(1);
    }
    
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr(SERVER_ADDRESS);
    serv_addr.sin_port = htons(PORT);
    
    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) == -1) {
        mvprintw(window_height - 1, 0, "Connection failed");
        // perror("Connection failed");
        close(sock);
        exit(EXIT_FAILURE);
    }

    if((received_bytes = recv(sock, msg, BUFFER_SIZE, 0)) <= 0){
        mvprintw(window_height - 1, 0, "Cannot get server status");
		// perror("Cannot get server status");
        close(sock);
        exit(EXIT_FAILURE);
	}

    msg[received_bytes] = '\0';
    if(strcmp(msg, "Server full") == 0){
        mvprintw(window_height - 1, 0, "Server is full!");
        refresh();
        close(sock);
        exit(EXIT_FAILURE);
    }
    
    send(sock, &function_choose, sizeof(function_choose), 0);

    int function_received;
    Server_response response;

    recv(sock, &function_received, sizeof(int), 0);

    switch(function_received)
    {
        case 0: // Will Receive Team_list via server_response union
        memset(&response, 0, sizeof(Server_response));

        Team_list team_list;
        if ((received_bytes = recv(sock, &team_list, sizeof(team_list), 0)) <= 0) {
            perror("Failed to receive team list");
            close(sock);
            exit(EXIT_FAILURE);
        }
        int check = 0;
        printf("%d\n", team_list.size);
        printf("%s %s\n", team_list.team_list[0], team_list.team_list[1]);
        while(check!=team_list.size){
            printf("While문 안임");
            printf("%d: %s\n", check, team_list.team_list[check]);
            check ++;
        }
            
        break;

        case 1: // will receive Team_detail via server_response union            
            send(sock, &req_data.team_detail, sizeof(Team_detail), 0);
	        memset(&response, 0, sizeof(Server_response));

            Team_detail new_team;
            if ((received_bytes = recv(sock, &new_team, sizeof(Team_detail), 0)) <= 0) {
                perror("Failed to receive team data");
                getchar();
                break;
            }
            break;

        case 2: // will receive Personal_table via server_response union

        break;

        case 3: // will receive Team_table via server_response union
        
        break;
    }
    
    close(sock); // close socket
}