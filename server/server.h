#ifndef SERVER_H
#define SERVER_H

#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <pthread.h>

#include "common.h"
#include "create_new_team.h"
#include "team_login_validator.h"
#include "team_list.h"
#include "user_login.h"
#include "make_team_table.h"
#include "manage_user_table.h"

#define PORT 8080
#define BUFFER_SIZE 1024
#define MAX_CLIENTS 8

#endif
