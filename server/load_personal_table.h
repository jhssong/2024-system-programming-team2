#ifndef LOAD_PERSONAL_TABLE_H
#define LOAD_PERSONAL_TABLE_H


#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <pthread.h>
#include <dirent.h>

#include <dirent.h>


#define TABLE_MAX_DAY 7
#define TABLE_MAX_TIME 26
#define MAX_CLIENTS 8
#define TEAMS_DIRECTORY "team/"

void load_personal_table(short table[MAX_CLIENTS][TABLE_MAX_TIME][TABLE_MAX_DAY], int *member_count, char *team_name);
void count_available_time(short table[MAX_CLIENTS][TABLE_MAX_TIME][TABLE_MAX_DAY], int member_count);

#endif