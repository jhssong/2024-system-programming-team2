#ifndef SERVER_H
#include <stdlib.h>
#include <sys/socket.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <pthread.h>

#define SERVER_H

#define APP_NAME "TTT_SERVER"
#define APP_VERSION "v0.0.1"

void connect_to_server();

#endif