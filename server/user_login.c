#include "user_login.h"

int revert_user_login(userinfo *new_user, int has_team_file, int has_user_file);
char* handle_user_login_error(char *error_msg, userinfo *new_user, int has_team_file, int has_user_file);

char* user_login(userinfo *new_user) {
#ifdef DEBUG
	printf("[DEBUG] User login.\n");
#endif

	struct stat statbuf;

	// Construct user folder path
	char user_folder_path[MAX_USER_FOLDER_PATH];
	snprintf(user_folder_path, sizeof(user_folder_path), "%s/%s/%s", TEAM_BASE_DIR, new_user->team_name, new_user->user_name);

	// Construct config file path
    char config_file_path[MAX_USER_CONFIG_FILE_PATH];
    snprintf(config_file_path, sizeof(config_file_path), "%s/%s", user_folder_path, USER_CONFIG_FILE_NAME);

	// Check user count
	char temp_user_folder_path[MAX_USER_FOLDER_PATH];
	snprintf(temp_user_folder_path, sizeof(temp_user_folder_path), "./%s/%s/", TEAM_BASE_DIR, new_user->team_name);

	DIR *user_folder_dir = opendir(temp_user_folder_path);
	if (user_folder_dir == NULL) {
        perror("Unable to open user folder directory");
        return handle_user_login_error("Error opening user folder directory", new_user, 0, 0);
    }
	struct dirent *entry;

	int user_count = 0;

	while((entry = readdir(user_folder_dir))!=NULL) {
		if (entry->d_name[0] != '.') {
      		char full_path[256];
			strncpy(full_path, "", sizeof(full_path) - strlen(full_path) - 1);
			strncat(full_path, temp_user_folder_path, sizeof(full_path) - strlen(full_path) - 1);
      		strncat(full_path, entry->d_name, sizeof(full_path) - strlen(full_path) - 1);
			
			if (stat(temp_user_folder_path, &statbuf) == 0 && S_ISDIR(statbuf.st_mode)) {
				if (strcmp(entry->d_name, new_user->user_name) == 0) {
					break;
				}
				user_count++;
      		}
    	}
  	}

	if (user_count > 8) {
		closedir(user_folder_dir);
		return handle_user_login_error("Error creating new user (reach the limit)", new_user, 0, 0);
	}

	// Check if the user folder already exists
    if (stat(user_folder_path, &statbuf) == 0) {
		// If exists
		int fd = open(config_file_path, O_RDONLY);
		if (fd < 0) {
			return handle_user_login_error("Error opening user config file", new_user, 0, 0);
		}
		#ifdef DEBUG
			printf("[DEBUG] Found existing user config file.\n");
		#endif

		char buffer[MAX_NAME_SIZE]; 
		ssize_t bytes_read;
		int i = 0;
		int is_name = 1;

		while ((bytes_read = read(fd, &buffer[i], 1)) > 0) {
			if (buffer[i] == '\n' || i == MAX_NAME_SIZE - 1) {
				buffer[i] = '\0';
				if (is_name) {
				#ifdef DEBUG
					printf("[DEBUG] Read user name: %s\n", buffer);
					is_name = 0;
				#endif
				}
				else {
				#ifdef DEBUG
					printf("[DEBUG] Read user pw: %s, input password: %s\n", buffer, new_user->user_pw);
				#endif
					close(fd);
					if (strcmp(buffer, new_user->user_pw) == 0) {
						return "Correct";
					} else return "User password incorrect";
					break;
				}
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

		return "Cannot open the user config file";
    }

	// Create user folder
	if (mkdir(user_folder_path, USER_FOLDER_MODE) < 0) {
        return handle_user_login_error("Error creating user directory", new_user, 0, 0);
    }
	else {
	#ifdef DEBUG
		printf("[DEBUG] New team directory created.\n");
	#endif
	}

    // Create and write to config file
    int fd = creat(config_file_path, USER_CONFIG_FILE_MODE);
    if (fd < 0) {
        return handle_user_login_error("Error creating user config file", new_user, 0, 0);
    }
	else {
	#ifdef DEBUG
		printf("[DEBUG] New user config file created.\n");
	#endif
	}

	// Write user_name (only the actual length)
	if (write(fd, new_user->user_name, strlen(new_user->user_name)) < 0) {
		close(fd);
		return handle_user_login_error("Error writing team name to config file", new_user, 0, 0);
	}
	write(fd, "\n", 1);

	// Write user_pw
	if (write(fd, new_user->user_pw, strlen(new_user->user_pw)) < 0) {
		close(fd);
		return handle_user_login_error("Error writing team password to config file", new_user, 0, 0);
	}
	write(fd, "\n", 1);

	#ifdef DEBUG
		printf("[DEBUG] User login successfully.\n");
		printf("[DEBUG]     name: %s\n", new_user->user_name);
		printf("[DEBUG]     pw:   %s\n", new_user->user_pw);
	#endif

    close(fd);

    return "Correct";
}

int revert_user_login(userinfo *new_user, int has_team_file, int has_user_file) {
	char user_folder_path[MAX_USER_FOLDER_PATH];
	sprintf(user_folder_path, "%s/%s/%s", TEAM_BASE_DIR, new_user->team_name, new_user->user_name);

	if (has_team_file) {
		char config_file_path[MAX_USER_CONFIG_FILE_PATH];
		sprintf(config_file_path, "%s/%s", user_folder_path, USER_CONFIG_FILE_NAME);

		if (unlink(config_file_path) != 0) {
			perror("Failed to delete file");
			return -1;
		}
	}

	if (has_user_file && rmdir(user_folder_path) != 0) {
		perror("Failed to delete directory");
		return -1;
	}

	return 0;
}

char* handle_user_login_error(char *error_msg, userinfo *new_user, int has_team_file, int has_user_file) {
    perror(error_msg);
    if (revert_user_login(new_user, has_team_file, has_user_file) != 0) {
        perror("Error during cleanup");
		return "Error during cleanup";
    }
	return error_msg;
}