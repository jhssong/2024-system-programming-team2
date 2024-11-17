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
    

    while(1) {
        printf("Enter message: ");
        fgets(msg, BUFFER_SIZE, stdin);
        msg[strcspn(msg, "\n")] = 0; // exchange '\n' to '\0'

        send(sock, msg, strlen(msg), 0);

        int bytes_received = recv(sock, msg, BUFFER_SIZE, 0);
        if(bytes_received <= 0){
            printf("Server disconnected\n");
            break;
        }

        msg[bytes_received] = '\0';
        printf("Server response: %s\n", msg);
    }
    
    close(sock); // close socket
    return 0;
}