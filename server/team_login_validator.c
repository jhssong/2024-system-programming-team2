#include "team_login_validator.h"


char* team_login_validator(teaminfo *new_team) {
	struct stat statbuf;

	// Check and create TEAM_BASE_DIR
	if (stat(TEAM_BASE_DIR, &statbuf) == -1) {
		if ((mkdir(TEAM_BASE_DIR, TEAM_FOLDER_MODE)) < 0) {
			perror("Error creating team base directory");
			return "Error creating team base directory";
		}
	} 
	else if (!S_ISDIR(statbuf.st_mode)) {
        fprintf(stderr, "TEAM_BASE_DIR is not a directory\n");
		return "TEAM_BASE_DIR is not a directory";
    }

	// Construct team folder path
	char team_folder_path[MAX_TEAM_FOLDER_PATH];
	snprintf(team_folder_path, sizeof(team_folder_path), "%s/%s", TEAM_BASE_DIR, new_team->team_name);

	// Check if the team folder already exists
    if (stat(team_folder_path, &statbuf) != 0) {
        fprintf(stderr, "Team directory '%s' doesn't exists\n", new_team->team_name);
		return "Team directory doesn't exists";
    }

	// Construct config file path
    char config_file_path[MAX_TEAM_CONFIG_FILE_PATH];
    snprintf(config_file_path, sizeof(config_file_path), "%s/%s", team_folder_path, TEAM_CONFIG_FILE_NAME);

	int fd = open(config_file_path, O_RDONLY);
	if (fd < 0) {
		return "Error opening team config file";
	}
	#ifdef DEBUG
		printf("[DEBUG] Found existing user config file.\n");
	#endif

	char buffer[MAX_NAME_SIZE];
	ssize_t bytes_read;
	int i = 0;
	int read_row = 0;

	while ((bytes_read = read(fd, &buffer[i], 1)) > 0) {
		if (buffer[i] == '\n' || i == MAX_NAME_SIZE - 1) {
			buffer[i] = '\0';
			switch(read_row) {
				case 0:
				#ifdef DEBUG
					printf("[DEBUG] Read name: %s\n", buffer);
				#endif
					break;
				case 1:
				#ifdef DEBUG
					printf("[DEBUG] Read password: %s, input password: %s\n", buffer, new_team->team_pw);
				#endif
					close(fd);
					if (strcmp(buffer, new_team->team_pw) == 0) {
						return "Correct";
					} else return "Team password incorrect";
					break;
				case 2:
				#ifdef DEBUG
					printf("[DEBUG] Read created time: %s\n", buffer);
				#endif
					break;
			}
			read_row++;
			i = 0; 
		} else {
			i++;
		}
	}

#ifdef DEBUG
	if (bytes_read == -1) {
		perror("Error reading file");
		close(fd);
		return "Error reading file";
	}

	if (bytes_read == 0) {
		printf("End of file reached.\n");
	}
#endif

	return "Cannot open team config file";
}