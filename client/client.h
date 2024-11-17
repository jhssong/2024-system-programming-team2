#ifndef CLIENT_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define CLIENT_H

#define APP_NAME "TTT"
#define APP_VERSION "v0.0.1"

void connect_to_server();

#endif