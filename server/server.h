#ifndef SERVER_H
#define SERVER_H

#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <pthread.h>

#include "common.h"
#include "create_new_team.h"
#include "team_login_validator.h"
#include "manage_personal_table.h"
#include "team_list.h"
#include "user_login.h"

#define PORT 8080
#define BUFFER_SIZE 1024
#define MAX_CLIENTS 8

#endif
