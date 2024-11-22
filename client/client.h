#ifndef CLIENT_H
#define CLIENT_H

#include <arpa/inet.h>
#include <sys/socket.h>
#include "common.h"
#include "personal_time_table.h"

#define SERVER_ADDRESS "127.0.0.1"
#define PORT 8080
#define BUFFER_SIZE 100

response connect_to_server(request_packet req);
void old_connect_to_server(int function_choose, Server_response req_data);

#endif
