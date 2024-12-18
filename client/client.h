#ifndef CLIENT_H
#define CLIENT_H

#include <arpa/inet.h>
#include <sys/socket.h>
#include "common.h"

#define SERVER_ADDRESS "127.0.0.1"
#define PORT 8080
#define BUFFER_SIZE 100

response_packet connect_to_server(request_packet req);

#endif
