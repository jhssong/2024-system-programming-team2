#ifndef CLIENT_H
#define CLIENT_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include "common.h"
#include "personal_time_table.h"

#define SERVER_ADDRESS "127.0.0.1"
#define PORT 8080
#define BUFFER_SIZE 100

void connect_to_server(int function_choose, Server_response req_data);

#endif
