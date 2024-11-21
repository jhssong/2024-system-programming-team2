#include "create_new_team.h"

int revert_create_team(char *team_name, int has_file);
void handle_creation_error(const char *error_msg, char *team_name, int has_file);

Team_detail* create_new_team(Team_detail *new_team) {
	#ifdef DEBUG
		printf("[DEBUG] Creating new team.\n");
	#endif

	struct stat statbuf;

	// Check and create TEAM_BASE_DIR
	if (stat(TEAM_BASE_DIR, &statbuf) == -1) {
		if ((mkdir(TEAM_BASE_DIR, TEAM_FOLDER_MODE)) < 0) {
			perror("Error creating team base directory");
			return NULL;
		}
	} 
	else if (!S_ISDIR(statbuf.st_mode)) {
        fprintf(stderr, "TEAM_BASE_DIR is not a directory\n");
        return NULL;
    }

	// Construct team folder path
	char team_folder_path[MAX_TEAM_FOLDER_PATH];
	snprintf(team_folder_path, sizeof(team_folder_path), "%s/%s", TEAM_BASE_DIR, new_team->team_name);

	// Check if the team folder already exists
    if (stat(team_folder_path, &statbuf) == 0) {
        fprintf(stderr, "Error: Team directory '%s' already exists\n", new_team->team_name);
        return NULL;
    }

	// Create team folder
    if (mkdir(team_folder_path, TEAM_FOLDER_MODE) < 0) {
        handle_creation_error("Error creating team directory", new_team->team_name, 0);
		return NULL;
    }
	else {
		#ifdef DEBUG
			printf("[DEBUG] New team directory created.\n");
		#endif
	}

	// Construct config file path
    char config_file_path[MAX_TEAM_CONFIG_FILE_PATH];
    snprintf(config_file_path, sizeof(config_file_path), "%s/%s", team_folder_path, TEAM_CONFIG_FILE_NAME);

    // Create and write to config file
    int fd = creat(config_file_path, TEAM_CONFIG_FILE_MODE);
    if (fd < 0) {
        handle_creation_error("Error creating team config file", new_team->team_name, 0);
		return NULL;
    }
	else {
		#ifdef DEBUG
			printf("[DEBUG] New team config file created.\n");
		#endif
	}

	// Write team_id
	int random_id = generate_random_number();
	char team_id[MAX_TEAM_ID_LEN];
	sprintf(team_id, "%d", random_id);
	team_id[MAX_TEAM_ID_LEN - 1] = '\0';
	strncpy(new_team->team_id, team_id, MAX_TEAM_ID_LEN);

	if (write(fd, new_team->team_id, strlen(new_team->team_id)) < 0) {
		close(fd);
		handle_creation_error("Error writing team id to config file", new_team->team_name, 1);
		return NULL;
	}
	write(fd, "\n", 1);

	// Write team_name (only the actual length)
	if (write(fd, new_team->team_name, strlen(new_team->team_name)) < 0) {
		close(fd);
		handle_creation_error("Error writing team name to config file", new_team->team_name, 1);
		return NULL;
	}
	write(fd, "\n", 1);

	// Write team_pw
	if (write(fd, new_team->team_pw, strlen(new_team->team_pw)) < 0) {
		close(fd);
		handle_creation_error("Error writing team password to config file", new_team->team_name, 1);
		return NULL;
	}
	write(fd, "\n", 1);

	// Write created_time
	time_t now = time(NULL);
	struct tm *timeinfo = localtime(&now);
	if (timeinfo == NULL) {
		close(fd);
		handle_creation_error("Error converting time", new_team->team_name, 1);
		return NULL;
	}

	char datetime_str[CREATED_TIME_LEN]; // ISO format: YYYY-MM-DDTHH:MM:SS
    strftime(datetime_str, CREATED_TIME_LEN, "%Y-%m-%dT%H:%M:%S", timeinfo);
	strncpy(new_team->created_time, datetime_str, CREATED_TIME_LEN);
	new_team->created_time[CREATED_TIME_LEN - 1] = '\0';

    if (write(fd, new_team->created_time, strlen(new_team->created_time)) < 0) {
        close(fd);
        handle_creation_error("Error writing date to team config file", new_team->team_name, 1);
        return NULL;
    }
	write(fd, "\n", 1);

	#ifdef DEBUG
		printf("[DEBUG] New team created successfully.\n");
		printf("[DEBUG]     id:           %s\n", new_team->team_id);
		printf("[DEBUG]     name:         %s\n", new_team->team_name);
		printf("[DEBUG]     pw:           %s\n", new_team->team_pw);
		printf("[DEBUG]     created time: %s\n", new_team->created_time);
	#endif

    close(fd);

	// TODO Add log on server terminal

	// FIXME Try not to use malloc!	
    Team_detail *result = malloc(sizeof(Team_detail));
    if (result) {
        memcpy(result, new_team, sizeof(Team_detail));
    }
    return result;
}

int revert_create_team(char *team_name, int has_file) {
	char team_folder_path[MAX_TEAM_FOLDER_PATH];
	sprintf(team_folder_path, "%s/%s", TEAM_BASE_DIR, team_name);		// Create new_team folder with name

	if (has_file) {
		char config_file_path[MAX_TEAM_CONFIG_FILE_PATH];
		sprintf(config_file_path, "%s/%s", team_folder_path, TEAM_CONFIG_FILE_NAME);

		if (unlink(config_file_path) != 0) {
			perror("Failed to delete file");
			return -1;
		}
	}

	if (rmdir(team_folder_path) != 0) {
		perror("Failed to delete directory");
		return -1;
	}

	return 0;
}

void handle_creation_error(const char *error_msg, char *team_name, int has_file) {
    perror(error_msg);
    if (team_name && revert_create_team(team_name, has_file) != 0) {
        perror("Error during cleanup");
    }
}
