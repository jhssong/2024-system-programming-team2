#include "server.h"

int client_count = 0;

void *handle_client(void *thread_sock) {
	int sock = *(int *)thread_sock;
	free(thread_sock);

	request_packet req;
	response_packet res;
	char* result;
    int received_bytes = recv(sock, &req, sizeof(request_packet), 0);

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
			int check = 0;
			#ifdef DEBUG
			while(check<res.res.team_list.size){
				printf("%d %s\n", check++, res.res.team_list.team_list[check]);
			}
			#endif

			if (send(sock, &res, sizeof(res), 0)<= 0) {
				perror("Failed to send team list response");
			}
		break;

		case 2:
			teaminfo new_team;
			memset(&new_team, 0, sizeof(teaminfo));
			new_team = req.req.team_info;

            result = create_new_team(&new_team);
            
			if (strcmp(result, "Success") == 0) {
				res.status_code = 200;
				strcpy(res.msg, "Success");

				// TODO Add response struct here @p1utie

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
		case 3:
	            char team_name[MAX_NAME_SIZE];
	            strcpy(team_name, req.req.user_table.team_name);
	
	            short personal_table[MAX_CLIENTS][TABLE_MAX_TIME][TABLE_MAX_DAY] = {0};
	            int member_count = 0;
	
	            make_team_table(personal_table, &member_count, team_name);
	            count_available_time(personal_table, member_count);
	
	            res_packet.status_code = 200;
	            snprintf(res_packet.msg, sizeof(res_packet.msg), "Team table created successfully");
	            memcpy(res.res.team_table, team_table, sizeof(team_table));
	
	            if (send(sock, &res_packet, sizeof(res_packet), 0) <= 0) {
	                perror("Failed to send team table response");
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
			} else {
			#ifdef DEBUG
				printf("[DEBUG] result: %s\n", result);
			#endif
				perror("Failed to create user");
				res.status_code = 503;
				strcpy(res.msg, "Failed to create user");
			}

			if (send(sock, &res, sizeof(response_packet), 0) <= 0) {
                perror("Failed to send user creation response");
            }
			break;
			
			// TODO Add response struct here @p1utie
			
		case 5: // server receive Personal_Time_Table and send Team_Time_Table to client
			Personal_Table personal_table;
			if((received_bytes = recv(sock, &personal_table, sizeof(Personal_Table), 0)) <= 0){
				perror("Failed to receive personal table data");
                close(sock);
                client_count--;
                return NULL;
			}else {
				#ifdef DEBUG
					printf("[DEBUG] Recieved data size: %zu\n", sizeof(Personal_Table));
				#endif
			}

			update_personal_table(&personal_table);	// at manage_personal_table.c

			//TODO: make Team_Time_Table and send to client
			char team_name[MAX_NAME_SIZE];
	                strcpy(team_name, req.req.user_table.team_name);
			short personal_tables[MAX_CLIENTS][TABLE_MAX_TIME][TABLE_MAX_DAY] = {0};
		        int member_count = 0;
	
		        make_team_table(personal_tables, &member_count, personal_table.team_name);
		        count_available_time(personal_tables, member_count);
			
		        res.status_code = 200;
		        memcpy(res.res.team_table, team_table, sizeof(team_table));
		
		        if (send(sock, &res, sizeof(response_packet), 0) <= 0) {
		            perror("Failed to send team table response");
		        } 
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

