#include "client.h"

response_packet connect_to_server(request_packet req) {
    int sock;
    int received_bytes;
    struct sockaddr_in serv_addr;

    response_packet res;

    sock = socket(PF_INET, SOCK_STREAM, 0);
    if(sock == -1){
        exit(EXIT_FAILURE);
    }

    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr(SERVER_ADDRESS);
    serv_addr.sin_port = htons(PORT);

    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) == -1) {
        echo();
        close(sock);

        move(window_height - 1, 0);
        addstr("Server closed. Press any key to exit...");
        refresh();
        getchar();
        endwin();
        exit(EXIT_SUCCESS);
    }
    
    received_bytes = recv(sock, &res, sizeof(response_packet), 0);

    if (received_bytes <= 0) {
        mvprintw(window_height - 1, 0, "Failed to receive data\n");
        refresh();
        echo();
        close(sock);
        exit(EXIT_FAILURE);
    }

    if (res.status_code == 503) {
        mvprintw(window_height - 1, 0, "[ERROR] 503 error: %s\n", res.msg);
        refresh();
        echo();
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
        echo();
        close(sock);
        exit(EXIT_FAILURE);
    }

    if (res.status_code == 503) {
        mvprintw(window_height - 1, 0, "%s", res.msg);
        refresh();
        echo();
        close(sock);
    } else {
        close(sock);
    #ifdef DEBUG
        printw("\n[DEBUG] status %d with: %s\n", res.status_code, res.msg);
        refresh();
    #endif
        return res;
    }
    return res;
}