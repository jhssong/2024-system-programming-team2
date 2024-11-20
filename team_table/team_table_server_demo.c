#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <pthread.h>
#include <dirent.h>

#define SERVER_H

#define APP_NAME "TTT_SERVER"
#define APP_VERSION "v0.0.1"

void connect_to_server();

#define PORT 8080
#define BUFFER_SIZE 1024
#define MAX_CLIENTS 8
#define TABLE_MAX_DAY 7
#define TABLE_MAX_TIME 26 
#define USERS_DIRECTORY "./users/"  // Directory containing user directories

int clnt_count = 0;

void *handle_client(void *thread_sock);
void load_personal_table(short table[MAX_CLIENTS][TABLE_MAX_TIME][TABLE_MAX_DAY], int *member_count);
void count_available_time(short table[MAX_CLIENTS][TABLE_MAX_TIME][TABLE_MAX_DAY], int member_count, short team_table[TABLE_MAX_TIME][TABLE_MAX_DAY]);

int main() {
    int serv_sock, clnt_sock;
    struct sockaddr_in serv_addr, clnt_addr;
    socklen_t clnt_addr_size;

    // server set 127.0.0.1:8080
    serv_sock = socket(PF_INET, SOCK_STREAM, 0);
    if (serv_sock == -1) {
        perror("socket() error");
        exit(1);
    }

    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(PORT);

    if (bind(serv_sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) == -1) {
        perror("bind() error");
        close(serv_sock);
        exit(1);
    }

    if (listen(serv_sock, MAX_CLIENTS) == -1) {
        perror("listen() error");
        close(serv_sock);
        exit(1);
    }


    while (1) {
        clnt_addr_size = sizeof(clnt_addr);
        clnt_sock = accept(serv_sock, (struct sockaddr *)&clnt_addr, &clnt_addr_size);
        if (clnt_sock == -1) {
            perror("accept() error");
            continue;
        }
        
        if (clnt_count >= 8) {
    	perror("Server max client is 8!");
    	// send "Server full" message and disconnect
    	const char *bad = "Server full";
  	    send(clnt_sock, bad, strlen(bad), 0);
	    close(clnt_sock);
    	continue;
		}
        /*
        const char *good = "Server is not full";
        send(clnt_sock, good, strlen(good), 0);
        */

		printf("New connection accepted.\n");
		clnt_count++;
        

        pthread_t thread_id;
        int *thread_sock = malloc(sizeof(int));
        *thread_sock = clnt_sock;
        if (pthread_create(&thread_id, NULL, handle_client, thread_sock) != 0) {
            perror("Could not create thread");
            free(thread_sock);
            close(clnt_sock);
            clnt_count--;
        }

        pthread_detach(thread_id);
    }

    close(serv_sock);
    return 0;
}

void *handle_client(void *thread_sock) {
    int sock = *(int *)thread_sock;
    free(thread_sock);

    char buffer[BUFFER_SIZE];
    int bytes_read;

    // Load and compute the team table
    short personal_table[MAX_CLIENTS][TABLE_MAX_TIME][TABLE_MAX_DAY] = {0};
    short team_table[TABLE_MAX_TIME][TABLE_MAX_DAY] = {0};
    int member_count = 0;

    load_personal_table(personal_table, &member_count);
    count_available_time(personal_table, member_count, team_table);
    
    // Handle client request and send the team table
    while ((bytes_read = recv(sock, buffer, BUFFER_SIZE, 0)) > 0) {
        buffer[bytes_read] = '\0';
        printf("Received request: %s\n", buffer);

        /*
        char *response = buffer;
		strcat(buffer, " is successfully send to server");
		send(sock, response, strlen(response), 0);
        */
        if (strcmp(buffer, "GET_TEAM_TABLE") == 0) {
            // Send the data of team_table
            send(sock, team_table, sizeof(team_table), 0);
        } else {
            const char *unknown = "Unknown command.";
            send(sock, unknown, strlen(unknown), 0);
        }
    }

    printf("Client disconnected.\n");
    close(sock);
    clnt_count--;
    return NULL;
}


// Load personal table from files in subdirectories
void load_personal_table(short table[MAX_CLIENTS][TABLE_MAX_TIME][TABLE_MAX_DAY], int *member_count) {
    DIR *dir, *subdir;  // Pointers for directories (main and subdirectory)
    struct dirent *entry, *subentry;  // Directory entry structures for reading files
    *member_count = 0;  // Initialize member count to 0

    // Attempt to open the users directory
    if ((dir = opendir(USERS_DIRECTORY)) == NULL) {
        perror("Cannot open users directory");  // Print error if directory cannot be opened
        return;  // Exit the function
    }

    // Iterate over entries in the users directory, limiting to MAX_CLIENTS
    while ((entry = readdir(dir)) != NULL && *member_count < MAX_CLIENTS) {
        // Check if the entry is a directory and not "." or ".."
        if (entry->d_type == DT_DIR && strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0) {
            char subdirectory_path[4096];  // Buffer for constructing subdirectory path
            // Create the path to the subdirectory
            snprintf(subdirectory_path, sizeof(subdirectory_path), "%s%s/", USERS_DIRECTORY, entry->d_name);

            // Attempt to open the subdirectory
            if ((subdir = opendir(subdirectory_path)) != NULL) {
                // Iterate over entries in the subdirectory
                while ((subentry = readdir(subdir)) != NULL) {
                    // Check if the file is named "personal_table.txt"
                    if (strcmp(subentry->d_name, "personal_table.txt") == 0) {
                        char filepath[4096];  // Buffer for constructing file path
                        // Create the path to the file
                        if (snprintf(filepath, sizeof(filepath), "%s%s", subdirectory_path, subentry->d_name) >= sizeof(filepath)) {
                            fprintf(stderr, "Warning: Path length exceeds limit: %s%s\n", subdirectory_path, subentry->d_name);
                            continue;  // Skip files with excessively long paths
                        }

                        // Open the file
                        FILE *file = fopen(filepath, "r");
                        if (file == NULL) {  // Handle file open error
                            perror("Cannot open table file");
                            continue;  // Skip to the next file
                        }

                        // Read the data into the table for this member
                        for (int t = 0; t < TABLE_MAX_TIME; t++) {
                            for (int d = 0; d < TABLE_MAX_DAY; d++) {
                                fscanf(file, "%hd", &table[*member_count][t][d]);
                            }
                        }
                        fclose(file);  // Close the file after reading
                        (*member_count)++;  // Increment the member count
                        break;  // Exit after processing one file per user
                    }
                }
                closedir(subdir); 
            }
        }
    }
    closedir(dir); 
}

// Count the number of available team members for each time slot
void count_available_time(short table[MAX_CLIENTS][TABLE_MAX_TIME][TABLE_MAX_DAY], int member_count, short team_table[TABLE_MAX_TIME][TABLE_MAX_DAY]) {
    for (int i = 0; i < TABLE_MAX_TIME; i++) {
        for (int j = 0; j < TABLE_MAX_DAY; j++) {
            team_table[i][j] = 0;
            for (int k = 0; k < member_count; k++) {
                if (table[k][i][j] == 1) {
                    team_table[i][j]++;
                }
            }
        }
    }
}