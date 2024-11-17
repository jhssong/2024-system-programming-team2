#include "client.h"

#define PORT 8080
#define BUFFER_SIZE 100

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
    recv(sock, &function_received, sizeof(int), 0);

    switch(function_received)
    {
        case 0: // Will Receive Team_list via server_response union

        break;

        case 1: // will receive Team_detail via server_response union

        break;

        case 2: // will receive Personal_table via server_response union

        break;

        case 3: // will receive Team_table via server_response union
        
        break;
    }
    
    close(sock); // close socket
    return 0;
}