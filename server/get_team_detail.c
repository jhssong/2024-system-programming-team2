#include "get_team_detail.h"


Team_detail get_team_detail(char *team_name) {
	struct stat statbuf;

	if (stat(TEAM_BASE_DIR, &statbuf) == -1) {			// Check is there team/ folder
		mkdir(TEAM_BASE_DIR, TEAM_FOLDER_MODE);
	}

	char team_folder_path[MAX_TEAM_FOLDER_PATH];
	sprintf(team_folder_path, "%s/%s", TEAM_BASE_DIR, team_name);

	mkdir(team_folder_path, TEAM_FOLDER_MODE);

	char config_file_path[MAX_TEAM_CONFIG_FILE_PATH];
	sprintf(config_file_path, "%s/%s", team_folder_path, TEAM_CONFIG_FILE_NAME);

	int fd = creat(config_file_path, TEAM_CONFIG_FILE_MODE);
	if (fd < 0)
	{
		perror("Error creating team config file ");
		exit(-1);
	}
}