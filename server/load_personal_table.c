#include "make_team_table.h"

short team_table[TABLE_MAX_TIME][TABLE_MAX_DAY] = {0};

// Function to load personal schedule tables for all users in a specific team
void make_team_table(short table[MAX_CLIENTS][TABLE_MAX_TIME][TABLE_MAX_DAY], int *member_count, char *team_name) {
    DIR *users_dir, *user_dir;
    struct dirent *user_entry;
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

    // Change to the team directory
    if (chdir(team_path) != 0) {
        perror("Failed to change to specific team directory");
        return;
    }

    // Open the "users" directory inside the team directory
    if ((users_dir = opendir("users")) == NULL) {
        perror("Cannot open users directory");
        chdir(cwd); // Return to the original working directory
        return;
    }

    // Change to the "users" directory
    if (chdir("users") != 0) {
        perror("Failed to change to users directory");
        closedir(users_dir);
        chdir(cwd); // Return to the original working directory
        return;
    }

    // Iterate through each user's directory
    while ((user_entry = readdir(users_dir)) != NULL && *member_count < MAX_CLIENTS) {
        if (user_entry->d_type == DT_DIR && strcmp(user_entry->d_name, ".") != 0 && strcmp(user_entry->d_name, "..") != 0) {
            // Change to the user's directory
            if (chdir(user_entry->d_name) != 0) {
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

            // Read the user's schedule table from the "table.bin" file
            size_t read_count = fread(&table[*member_count], sizeof(short), TABLE_MAX_TIME * TABLE_MAX_DAY, file);
            if (read_count != TABLE_MAX_TIME * TABLE_MAX_DAY) {
                printf("Warning: Incomplete data in table.bin for user %s. Read %zu elements.\n",
                       user_entry->d_name, read_count);
            } else {
                printf("Loaded table.bin for user %s.\n", user_entry->d_name);
                (*member_count)++;
            }

            fclose(file);
            chdir(".."); // Go back to the parent directory
        }
    }

    // Close the users directory and return to the original working directory
    closedir(users_dir);
    chdir(cwd);
}

// Function to count the available times for the team based on individual schedules
void count_available_time(short table[MAX_CLIENTS][TABLE_MAX_TIME][TABLE_MAX_DAY], int member_count) {
    if (member_count == 0) {
        printf("No members to process. Team table will be empty.\n");
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
