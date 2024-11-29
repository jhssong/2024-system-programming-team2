#include "make_team_table.h"

// Function to load personal schedule tables for all users in a specific team
void make_team_table(short table[MAX_CLIENTS][TABLE_MAX_TIME][TABLE_MAX_DAY], int *member_count, char *team_name) {
    DIR *team_dir;//, *user_dir;
    struct dirent *entry;
    *member_count = 0;

    // Save the current working directory
    char cwd[4096];
    if (getcwd(cwd, sizeof(cwd)) == NULL) {
        perror("Failed to get current working directory");
        return;
    }

    // Create the team directory path
    char team_path[512];
    snprintf(team_path, sizeof(team_path), "%s%s", TEAMS_DIRECTORY, team_name);

    // Open the team directory
    if ((team_dir = opendir(team_path)) == NULL) {
        perror("Cannot open team directory");
        return;
    }

    // Change to the team directory
    if (chdir(team_path) != 0) {
        perror("Failed to change to team directory");
        closedir(team_dir);
        return;
    }

    // Iterate through user directories
    while ((entry = readdir(team_dir)) != NULL && *member_count < MAX_CLIENTS) {
        if (entry->d_type == DT_DIR && strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0) {
            // Change to the user directory
            if (chdir(entry->d_name) != 0) {
                perror("Failed to change to user directory");
                continue;
            }

            // Open the "table.bin" file inside the user's directory
            FILE *file = fopen("table.bin", "rb");
            if (file == NULL) {
                perror("Cannot open table.bin in user directory");
                chdir(".."); // Go back to the parent directory
                continue;
            }

            // Read data from table.bin" 
            size_t read_count = fread(&table[*member_count], sizeof(short), TABLE_MAX_TIME * TABLE_MAX_DAY, file);
            if (read_count != TABLE_MAX_TIME * TABLE_MAX_DAY) {
                #ifdef DEBUG
                    printf("Warning: Incomplete data in table.bin for user %s. Read %zu elements.\n",
                        entry->d_name, read_count);
                #endif
            } else {
                #ifdef DEBUG
                    printf("Loaded table.bin for user %s.\n", entry->d_name);
                #endif
                (*member_count)++;
            }

            fclose(file);
            chdir(".."); // Go back to the parent directory
        }
    }

    // Close the team directory and return to the original working directory
    closedir(team_dir);
    chdir(cwd);
}


// Function to count the available times for the team based on individual schedules
void count_available_time(short table[MAX_CLIENTS][TABLE_MAX_TIME][TABLE_MAX_DAY], int member_count, short team_table[TABLE_MAX_TIME][TABLE_MAX_DAY]) {
    if (member_count == 0) {
        #ifdef DEBUG
            printf("No members to process. Team table will be empty.\n");
        #endif
        return;
    }

    // Iterate through each time slot and day
    for (int i = 0; i < TABLE_MAX_TIME; i++) {
        for (int j = 0; j < TABLE_MAX_DAY; j++) {
            team_table[i][j] = 0; // Initialize the team table cell to 0

            // Sum up the availability from all members
            for (int k = 0; k < member_count; k++) {
                if (table[k][i][j] == 1) { // Check if the member is available
                    team_table[i][j]++;
                }
            }
        }
    }
}