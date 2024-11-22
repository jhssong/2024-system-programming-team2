#include "client.h"

void connect_to_server(int function_choose, Server_response req_data) {
    int sock;
    struct sockaddr_in serv_addr;

    int received_bytes;
    char msg[BUFFER_SIZE];
    
    sock = socket(PF_INET, SOCK_STREAM, 0);
    if(sock == -1){
        perror("socket() error");
        exit(1);
    }
    
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr(SERVER_ADDRESS);
    serv_addr.sin_port = htons(PORT);
    
    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) == -1) {
        perror("Connection failed");
        close(sock);
        exit(1);
    }

    if((received_bytes = recv(sock, msg, BUFFER_SIZE, 0)) <= 0){
		perror("Cannot get server status");
        close(sock);
        exit(1);
	}

    msg[received_bytes] = '\0';
    if(strcmp(msg, "Server full") == 0){
        printw("\n\nServer is full!\nPlease wait until other client disconnect server!");
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