#include "server.h"

#define PORT 8080
#define BUFFER_SIZE 1024
#define MAX_CLIENTS 8

int clnt_count = 0;

Server_response function_0 (){
	Server_response team_directory;

	// get team_directory func

	return team_directory;
}

void *handle_client(void *thread_sock)
{
	int sock = *(int *)thread_sock;
	free(thread_sock);

	// char buffer[BUFFER_SIZE]; // receive info that client send(it is not used while function has completed)
	int function_choose; // receive what func do
	int bytes_read;


	if((bytes_read = recv(sock, &function_choose, sizeof(int), 0)) < 0){
		perror("Cannot receive what func do");
		exit(EXIT_FAILURE) ; // Disconnect 
	}

	send(sock, &function_choose, sizeof(int), 0); // send client what client send

	Server_response response;
	memset(&response, 0, sizeof(Server_response));
	
	// below function that server send to client must be return type sever_response union!
	switch(function_choose){
		case 0: // server send Team_List to client
			response = function_0();
			send(sock, &response, sizeof(Server_response), 0);
		break;

		case 1: // server send Team_Detail to client

		break;

		case 2: // server send Personal_Time_Table to client

		break;

		case 3: // server send Team_Time_Table to client
		
		break;
	}
	printf("Client disconnected.\n");
	close(sock);
	clnt_count--;
	return NULL;
}

// server set 127.0.0.1:8080 
int main()
{
	int serv_sock, clnt_sock;
	struct sockaddr_in serv_addr, clnt_addr;
	socklen_t clnt_addr_size;

	serv_sock = socket(PF_INET, SOCK_STREAM, 0); // create socket
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
		
		if (clnt_count == 9) {
    		perror("Server max client is 8!");
    		// send "Server full" message and disconnect
    		const char *bad = "Server full";
  	  		send(clnt_sock, bad, strlen(bad), 0);
	    	close(clnt_sock);
    		continue;
		}else{
			const char *good = "Server is not full";
			send(clnt_sock, good, strlen(good), 0);
		}

		if((clnt_sock = accept(serv_sock, (struct sockaddr *)&clnt_addr, &clnt_addr_size)) == -1){
			perror("accept() error");
			continue;
		}

		printf("New connection accepted.\n");
		clnt_count++;

		pthread_t thread_id;
		int *thread_sock = malloc(sizeof(int));
		*thread_sock = clnt_sock;
		if (pthread_create(&thread_id, NULL, handle_client, thread_sock) != 0){
			perror("Could not create thread");
			clnt_count--;
			close(clnt_sock);
			free(thread_sock);
		}
		pthread_detach(thread_id);
	}

	close(serv_sock);
	return 0;
}