#include "server.h"

int client_count = 0;

Server_response function_0 () {
	Server_response team_directory;

	// get team_directory func

	return team_directory;
}

void *handle_client(void *thread_sock) {
	int sock = *(int *)thread_sock;
	free(thread_sock);

	int function_choose; // number that which function to execute
	int received_bytes;

	if((received_bytes = recv(sock, &function_choose, sizeof(int), 0)) <= 0){
		perror("Failed to receive function choice");
        close(sock);
        client_count--;
        return NULL;
	}

	// Send back the function_choose that server received
    if (send(sock, &function_choose, sizeof(int), 0) <= 0) {
        perror("Failed to send function choice");
        close(sock);
        client_count--;
        return NULL;
    }

	Server_response response;
	memset(&response, 0, sizeof(Server_response));
	
	// below function that server send to client must be return type sever_response union!
	switch(function_choose){
		case 0: 	// server send Team_List to client
			response = function_0();
			send(sock, &response, sizeof(Server_response), 0);
		break;

		case 1:		// create new team
			Team_detail new_team;
            if ((received_bytes = recv(sock, &new_team, sizeof(Team_detail), 0)) <= 0) {
                perror("Failed to receive team data");
                close(sock);
                client_count--;
                return NULL;
            }

            Team_detail *result = create_team(&new_team);
            if (result == NULL) {
                perror("Failed to create team");
            } else if (send(sock, result, sizeof(Team_detail), 0) <= 0) {
                perror("Failed to send team creation response");
            }

			free(result);
            break;

		case 2: // server send Personal_Time_Table to client

		break;

		case 3: // server send Team_Time_Table to client
		
		break;
	}

	printf("Client disconnected.\n");
	close(sock);
	client_count--;
	return NULL;
}

int main()
{
	int serv_sock, clnt_sock;
	struct sockaddr_in serv_addr, clnt_addr;
	socklen_t clnt_addr_size;

	serv_sock = socket(PF_INET, SOCK_STREAM, 0);
	if (serv_sock == -1){
		perror("sock() error");
		exit(1);
	}

	memset(&serv_addr, 0, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	serv_addr.sin_port = htons(PORT);

	if (bind(serv_sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) == -1){
		perror("bind() error");
		close(serv_sock);
		exit(1);
	}

	if (listen(serv_sock, 8) == -1){
		perror("listen() error");
		close(serv_sock);
		exit(1);
	}

	while(1){
		clnt_addr_size = sizeof(clnt_addr);
        if ((clnt_sock = accept(serv_sock, (struct sockaddr *)&clnt_addr, &clnt_addr_size)) == -1) {
            perror("accept() error");
            continue;
        }

        if (client_count >= 8) {
            const char *bad = "Server full";
            send(clnt_sock, bad, strlen(bad), 0);
            close(clnt_sock);
            continue;
        } else {
			const char *good = "Connected!";
            send(clnt_sock, good, strlen(good), 0);
		}

		client_count++;
		printf("New connection accepted.\n");

		pthread_t thread_id;
		int *thread_sock = malloc(sizeof(int));
		*thread_sock = clnt_sock;
		if (pthread_create(&thread_id, NULL, handle_client, thread_sock) != 0){
			perror("Could not create thread");
			client_count--;
			close(clnt_sock);
			free(thread_sock);
		}
		pthread_detach(thread_id);
	}

	close(serv_sock);
	return 0;
}