#include "create_team.h"

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

Team_detail* create_team(Team_detail *new_team) {
	struct stat statbuf;

	// Check and create TEAM_BASE_DIR
	if (stat(TEAM_BASE_DIR, &statbuf) == -1) {
		if ((mkdir(TEAM_BASE_DIR, TEAM_FOLDER_MODE)) < 0) {
			perror("Error creating team base directory");
			return NULL;
		}
	}

	// Construct team folder path
	char team_folder_path[MAX_TEAM_FOLDER_PATH];
	snprintf(team_folder_path, sizeof(team_folder_path), "%s/%s", TEAM_BASE_DIR, new_team->team_name);

	// Create team folder
    if (mkdir(team_folder_path, TEAM_FOLDER_MODE) < 0) {
        handle_creation_error("Error creating team directory", new_team->team_name, 0);
		return NULL;
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

	// Write team_name (only the actual length)
	size_t name_length = strlen(new_team->team_name);
	if (write(fd, new_team->team_name, name_length) < 0) {
		close(fd);
		handle_creation_error("Error writing team name to config file", new_team->team_name, 1);
		return NULL;
	}
	write(fd, "\n", 1);

	// Write team_pw
	if (write(fd, new_team->team_pw, MAX_TEAM_PW_LEN - 1) < 0) {
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

	char date_str[11]; // YYYY-MM-DD\0
	strftime(date_str, sizeof(date_str), "%Y-%m-%d", timeinfo);
	strcpy(new_team->created_time, date_str);

	if (write(fd, date_str, strlen(date_str)) < 0) {
		close(fd);
		handle_creation_error("Error writing date to team config file", new_team->team_name, 1);
		return NULL;
	}

    close(fd);

	// TODO Add log on server terminal

	// FIXME Try not to use malloc!	
    Team_detail *result = malloc(sizeof(Team_detail));
    if (result) {
        memcpy(result, new_team, sizeof(Team_detail));
    }
    return result;
}