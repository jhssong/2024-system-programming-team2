#include "team_list.h"

teamlist get_team_list() {

	struct stat statbuf;

	// Check and create TEAM_BASE_DIR
	if (stat(TEAM_BASE_DIR, &statbuf) == -1) {
		if ((mkdir(TEAM_BASE_DIR, TEAM_FOLDER_MODE)) < 0) {
			perror("Error creating team base directory");
		}
	} 
	else if (!S_ISDIR(statbuf.st_mode)) {
        fprintf(stderr, "TEAM_BASE_DIR is not a directory\n");
    }

	DIR *dir = opendir("./team");
	struct dirent *entry;
	teamlist current_exist_team_list;

	int i = 0;

	while((entry = readdir(dir))!=NULL) {
		if (entry->d_name[0] != '.') {
      		char full_path[256] = "./team/"; 
      		strncat(full_path, entry->d_name, sizeof(full_path) - strlen(full_path) - 1);
			
			if (stat(full_path, &statbuf) == 0 && S_ISDIR(statbuf.st_mode)) {
				strncpy(current_exist_team_list.team_list[i], entry->d_name, strlen(entry->d_name) + 1);
				i++;
      		}
    	}
  	}

	current_exist_team_list.size = i;
	closedir(dir);
	return current_exist_team_list;
}