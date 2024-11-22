#ifndef SERVER_H
#define SERVER_H

#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <pthread.h>

#include "common.h"
#include "create_team.h"
#include "get_team_detail.h"
#include "team_list.h"

#define PORT 8080
#define BUFFER_SIZE 1024
#define MAX_CLIENTS 8

#endif