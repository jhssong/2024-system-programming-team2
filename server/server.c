#include "server.h"

int client_count = 0;

void *handle_client(void *thread_sock) {
	int sock = *(int *)thread_sock;

	request_packet req;
	response_packet res;
    int received_bytes = recv(sock, &req, sizeof(request_packet), 0);

#ifdef DEBUG
	printf("[DEBUG] received bytes: %d\n", received_bytes);
#endif

	if (received_bytes <= 0) {
        perror("Failed to receive request_data1");
        close(sock);
		client_count--;
		free(thread_sock);
		return NULL;
    }

	printf("cmd: %d\n", req.cmd);

		
	switch(req.cmd){
		case 0: 	// server send Team_List to client
			// FIX FIx according to new data struct
			// Team_list team_list;
			// team_list = get_team_list();
			// if (send(sock, &team_list, sizeof(Team_list), 0)<= 0){
			// 	perror("Failed to send team list response");
			// }
		break;

		case 2:
		printf("[asdfasdf] adfafasdfafasfaCreating new team.\n");
		#ifdef DEBUG
			printf("[DEBUG] adfafasdfafasfaCreating new team.\n");
		#endif
			teaminfo new_team;
			memset(&new_team, 0, sizeof(teaminfo));
			new_team = req.req.team_info;

            char* result = create_new_team(&new_team);
            
			if (result == NULL) {
                perror("Failed to create team");
				res.status_code = 503;
				strcpy(res.msg, "Failed to create team");
            } else if (send(sock, &res, sizeof(response_packet), 0) <= 0) {
                perror("Failed to send team creation response");
            }

			free(result);
            break;
	}

	printf("Client disconnected.\n");
	close(sock);
	client_count--;
	free(thread_sock);
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