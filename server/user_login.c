#include "user_login.h"

int revert_user_login(userinfo *new_user, int has_file);
char* handle_user_login_error(char *error_msg, userinfo *new_user, int has_file);

char* user_login(userinfo *new_user) {
#ifdef DEBUG
	printf("[DEBUG] User login.\n");
#endif

	struct stat statbuf;

	// Construct user folder path
	char user_folder_path[MAX_USER_FOLDER_PATH];
	snprintf(user_folder_path, sizeof(user_folder_path), "%s/%s/%s", TEAM_BASE_DIR, new_user->team_name, new_user->user_name);

	// Check if the user folder already exists
    if (stat(user_folder_path, &statbuf) == 0) {
		// If exists

		// Read user config file
    }

	// Create user folder
	if (mkdir(user_folder_path, USER_FOLDER_MODE) < 0) {
        return handle_user_login_error("Error creating user directory", new_user, 0);
    }
	else {
	#ifdef DEBUG
		printf("[DEBUG] New team directory created.\n");
	#endif
	}

	// Construct config file path
    char config_file_path[MAX_USER_CONFIG_FILE_PATH];
    snprintf(config_file_path, sizeof(config_file_path), "%s/%s", user_folder_path, USER_CONFIG_FILE_NAME);

    // Create and write to config file
    int fd = creat(config_file_path, USER_CONFIG_FILE_MODE);
    if (fd < 0) {
        return handle_user_login_error("Error creating user config file", new_user, 0);
    }
	else {
	#ifdef DEBUG
		printf("[DEBUG] New user config file created.\n");
	#endif
	}

	// Write user_name (only the actual length)
	if (write(fd, new_user->user_name, strlen(new_user->user_name)) < 0) {
		close(fd);
		return handle_user_login_error("Error writing team name to config file", new_user, 0);
	}
	write(fd, "\n", 1);

	// Write user_pw
	if (write(fd, new_user->user_pw, strlen(new_user->user_pw)) < 0) {
		close(fd);
		return handle_user_login_error("Error writing team password to config file", new_user, 0);
	}
	write(fd, "\n", 1);

	#ifdef DEBUG
		printf("[DEBUG] User login successfully.\n");
		printf("[DEBUG]     name:         %s\n", new_user->user_name);
		printf("[DEBUG]     pw:           %s\n", new_user->user_pw);
	#endif

    close(fd);

    return "Success";
}

int revert_user_login(userinfo *new_user, int has_file) {
	char user_folder_path[MAX_USER_FOLDER_PATH];
	sprintf(user_folder_path, "%s/%s/%s", TEAM_BASE_DIR, new_user->team_name, new_user->user_name);

	if (has_file) {
		char config_file_path[MAX_USER_CONFIG_FILE_PATH];
		sprintf(config_file_path, "%s/%s", user_folder_path, USER_CONFIG_FILE_NAME);

		if (unlink(config_file_path) != 0) {
			perror("Failed to delete file");
			return -1;
		}
	}

	if (rmdir(user_folder_path) != 0) {
		perror("Failed to delete directory");
		return -1;
	}

	return 0;
}

char* handle_user_login_error(char *error_msg, userinfo *new_user, int has_file) {
    perror(error_msg);
    if (revert_user_login(new_user, has_file) != 0) {
        perror("Error during cleanup");
		return "Error during cleanup";
    }
	return error_msg;
}