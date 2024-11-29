#include "server.h"

int client_count = 0;

void *handle_client(void *thread_sock) {
	int sock = *(int *)thread_sock;
	free(thread_sock);

	request_packet req;
	response_packet res;
	char* result;
    int received_bytes = recv(sock, &req, sizeof(request_packet), 0);

	teaminfo new_team;

#ifdef DEBUG
	printf("[DEBUG] received bytes: %d\n", received_bytes);
#endif

	if (received_bytes <= 0) {
        perror("Failed to receive request_data1");
        close(sock);
		client_count--;
		return NULL;
    }

#ifdef DEBUG
	printf("[DEBUG] Received cmd: %d\n", req.cmd);
#endif
		
	switch(req.cmd){
		case 0:
			teamlist current_exist_team_list;
			memset(&current_exist_team_list, 0, sizeof(teamlist));
			current_exist_team_list = get_team_list();
			res.res.team_list = current_exist_team_list;
			res.status_code = 200;
		#ifdef DEBUG
			for (int i = 0; i < res.res.team_list.size; i++) {
				printf("[DEBUG] team #%d: %s\n", i, res.res.team_list.team_list[i]);
			}
		#endif

			if (send(sock, &res, sizeof(res), 0)<= 0) {
				perror("Failed to send team list response");
			}
		break;

		case 2:
			memset(&new_team, 0, sizeof(teaminfo));
			new_team = req.req.team_info;

            result = create_new_team(&new_team);
            
			if (strcmp(result, "Success") == 0) {
				res.status_code = 200;
				strcpy(res.msg, "Success");
			} else {
			#ifdef DEBUG
				printf("[DEBUG] result: %s\n", result);
			#endif
				perror("Failed to create team");
				res.status_code = 503;
				strcpy(res.msg, "Failed to create team");
			}

			if (send(sock, &res, sizeof(response_packet), 0) <= 0) {
                perror("Failed to send team creation response");
            }

            break;

		case 3:
			memset(&new_team, 0, sizeof(teaminfo));
			new_team = req.req.team_info;

            result = team_login_validator(&new_team);
		#ifdef DEBUG
			printf("[DEBUG] login validator result: %s\n", result);
		#endif


			if (strcmp(result, "Success") == 0) {
				res.status_code = 200;
				strcpy(res.msg, "Success");
			} else if (strcmp(result, "Correct") == 0) {
				res.status_code = 202;
				strcpy(res.msg, "Correct");

				//Send team_table only when the team login is successful
				memset(res.res.team_table, 0, sizeof(res.res.team_table));
				short user_tables[MAX_CLIENTS][TABLE_MAX_TIME][TABLE_MAX_DAY] = {0};
				int member_count = 0;
				make_team_table(user_tables, &member_count, req.req.team_info.team_name);
				count_available_time(user_tables, member_count, res.res.team_table);
			} else if (strcmp(result, "Team not found") == 0) {
				fprintf(stderr, "Failed to check the team password");
				res.status_code = 404;
				strcpy(res.msg, "Team not found");
			} else if (strcmp(result, "Team password incorrect") == 0) {
				fprintf(stderr, "Team password incorrect");
				res.status_code = 401;
				strcpy(res.msg, "Team password incorrect");
			} else {
			#ifdef DEBUG
				printf("[DEBUG] result: %s\n", result);
			#endif
				perror("Failed to check the team password");
				res.status_code = 503;
				strcpy(res.msg, "Failed to check the team password");
			}

			if (send(sock, &res, sizeof(response_packet), 0) <= 0) {
                perror("Failed to send team creation response");
            }

			break;

		case 4:
			userinfo new_user;
			memset(&new_user, 0, sizeof(userinfo));
			new_user = req.req.user_info;
			result = user_login(&new_user);
            
			if (strcmp(result, "Success") == 0) {
				res.status_code = 200;
				strcpy(res.msg, "Success");
			} else if (strcmp(result, "Correct") == 0) {
				res.status_code = 202;
				strcpy(res.msg, "Correct");

				//Send user_table only when the user login is successful
				memset(res.res.user_table, 0, sizeof(res.res.user_table));
				load_user_table_from_file(&new_user, res.res.user_table);
			} else if (strcmp(result, "User password incorrect") == 0) {
				fprintf(stderr, "User password incorrect\n");
				res.status_code = 401;
				strcpy(res.msg, "User password incorrect");
			} else if (strcmp(result, "Error creating new user (reach the limit)") == 0) {
				fprintf(stderr, "Error creating new user (reach the limit)\n");
				res.status_code = 507;
				strcpy(res.msg, "Error creating new user (reach the limit)");
			} else {
			#ifdef DEBUG
				printf("[DEBUG] result: %s\n", result);
			#endif
				perror("Failed to create user");
				res.status_code = 503;
				strcpy(res.msg, result);
			}

		#ifdef DEBUG
			printf("[DEBUG] User Login return\n");
			printf("[DEBUG] 	status: %d\n", res.status_code);
			printf("[DEBUG]        msg: %s\n", res.msg);
		#endif

			if (send(sock, &res, sizeof(response_packet), 0) <= 0) {
                perror("Failed to send user creation response");
            }
			break;

		case 5: // server receive Personal_Time_Table and send Team_Time_Table to client
			
			update_user_table new_user_table;
			memset(&new_user_table, 0, sizeof(update_user_table));
			new_user_table=req.req.user_table;
			#ifdef DEBUG
				printf("[DEBUG] received user_name: %s\n", new_user_table.user_name);
				printf("[DEBUG] received team_name: %s\n", new_user_table.team_name);
			#endif

			result = update_user_table_file(&new_user_table);

			if(strcmp(result, "Success") == 0){
				res.status_code = 200;
				strcpy(res.msg, "Success");
			} else {
			#ifdef DEBUG
				printf("[DEBUG] result: %s\n", result);
			#endif
				res.status_code = 404;
				strcpy(res.msg, result);
			}
			memset(res.res.team_table, 0, sizeof(res.res.team_table));
			short user_tables[MAX_CLIENTS][TABLE_MAX_TIME][TABLE_MAX_DAY] = {0};
			int member_count = 0;
			make_team_table(user_tables, &member_count, req.req.user_table.team_name);
			count_available_time(user_tables, member_count, res.res.team_table);

			if (send(sock, &res, sizeof(response_packet), 0) <= 0) {
                perror("Failed to send update user table's response");
            }
			break;	
	}

	printf("Client disconnected.\n");
	close(sock);
	client_count--;
	return NULL;
}

int main() {
	int server_sock, client_sock;
	struct sockaddr_in server_addr, client_addr;
	socklen_t client_addr_size;

	server_sock = socket(PF_INET, SOCK_STREAM, 0);
	if (server_sock == -1){
		perror("sock() error");
		exit(1);
	}

	memset(&server_addr, 0, sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	server_addr.sin_port = htons(PORT);

	if (bind(server_sock, (struct sockaddr*)&server_addr, sizeof(server_addr)) == -1){
		perror("bind() error");
		close(server_sock);
		exit(1);
	}

	if (listen(server_sock, 8) == -1){
		perror("listen() error");
		close(server_sock);
		exit(1);
	}

	while(1){
		client_addr_size = sizeof(client_addr);
        if ((client_sock = accept(server_sock, (struct sockaddr *)&client_addr, &client_addr_size)) == -1) {
            perror("accept() error");
            continue;
        }

		response_packet res;
		memset(&res, 0, sizeof(response_packet));

        if (client_count >= 8) {
			res.status_code = 503;
			strcpy(res.msg, "Server full");
            send(client_sock, &res, sizeof(response_packet), 0);
            close(client_sock);
            continue;
        } else {
			res.status_code = 200;
			strcpy(res.msg, "Connected");
            send(client_sock, &res, sizeof(response_packet), 0);
		}

		client_count++;
		printf("New connection accepted.\n");

		pthread_t thread_id;
		int *thread_sock = malloc(sizeof(int));
		*thread_sock = client_sock;
		if (pthread_create(&thread_id, NULL, handle_client, thread_sock) != 0){
			perror("Could not create thread");
			client_count--;
			close(client_sock);
			free(thread_sock);
		}
		pthread_detach(thread_id);
	}

	close(server_sock);
	return 0;
}
