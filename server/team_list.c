#include "team_list.h"

teamlist get_team_list() {
	DIR *dir = opendir("./team");
	struct dirent *entry;
	struct stat statbuf;
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