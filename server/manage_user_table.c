#include "manage_user_table.h"

void load_user_table_from_file(userinfo *new_user, short team_table[TABLE_MAX_TIME][TABLE_MAX_DAY]) {
    char file_path[MAX_USER_SCHEDULE_FILE_PATH];
    struct stat statbuf;

    // Construct the file path: TEAM_BASE_DIR/team_name/username/table.bin
    snprintf(file_path, sizeof(file_path), "%s/%s/%s/table.bin", TEAM_BASE_DIR, new_user->team_name, new_user->user_name);

    // Check if table.bin exists
    if (stat(file_path, &statbuf) == -1) { // If file does not exist:
        // Initialize schedule array to 0
        memset(team_table, 0, TABLE_MAX_TIME * TABLE_MAX_DAY * sizeof(short));

        // Create and write table.bin
        FILE *file = fopen(file_path, "wb");
        if (file == NULL) {
            perror("Error creating table.bin");
            return;
        }
        if (fwrite(team_table, sizeof(short), TABLE_MAX_TIME * TABLE_MAX_DAY, file) != TABLE_MAX_TIME * TABLE_MAX_DAY) {
            perror("Error writing to table.bin");
            fclose(file);
            return;
        }
        fclose(file);
        #ifdef DEBUG
            printf("[DEBUG] table.bin created and initialized to 0.\n");
        #endif
    } else { // If file exists:
        // Open table.bin for reading
        FILE *file = fopen(file_path, "rb");
        if (file == NULL) {
            perror("Error opening table.bin");
            return;
        }
        if (fread(team_table, sizeof(short), TABLE_MAX_TIME * TABLE_MAX_DAY, file) != TABLE_MAX_TIME * TABLE_MAX_DAY) {
            perror("Error reading from table.bin");
            fclose(file);
            return;
        }
        fclose(file);
        #ifdef DEBUG
            printf("[DEBUG] table.bin loaded successfully.\n");
        #endif
    }
}

char* update_user_table_file(update_user_table *new_user_table) {
    char file_path[MAX_USER_SCHEDULE_FILE_PATH];

    // Construct the file path: TEAM_BASE_DIR/team_name/username/table.bin
    snprintf(file_path, sizeof(file_path), "%s/%s/%s/table.bin", TEAM_BASE_DIR, new_user_table->team_name, new_user_table->user_name);

    // Open table.bin for writing (create or overwrite)
    FILE *file = fopen(file_path, "wb");
    if (file == NULL) {
        perror("Error opening table.bin for writing");
        return "Error opening table.bin for writing";
    }

    // Write data from team_table to table.bin
    if (fwrite(new_user_table->user_table, sizeof(short), TABLE_MAX_TIME * TABLE_MAX_DAY, file) != TABLE_MAX_TIME * TABLE_MAX_DAY) {
        perror("Error writing to table.bin");
        fclose(file);
        return "Error writing to table.bin";
    }

    fclose(file);
    #ifdef DEBUG
        printf("table.bin updated successfully from personal_table.schedule.\n");
    #endif
    return "Success";
}