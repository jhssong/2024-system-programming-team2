#include "manage_personal_table.h"

void send_personal_table(Personal_Table *personal_table) {
    char file_path[MAX_USER_SCHEDULE_FILE_PATH];
    struct stat statbuf;

    // Construct the file path: TEAM_BASE_DIR/team_name/username/schedule.bin
    snprintf(file_path, sizeof(file_path), "%s/%s/%s/schedule.bin", TEAM_BASE_DIR, personal_table->team_name, personal_table->username);

    // Check if schedule.bin exists
    if (stat(file_path, &statbuf) == -1) { // If file does not exist:
        // Initialize schedule array to 0
        memset(personal_table->schedule, 0, sizeof(personal_table->schedule));

        // Create and write schedule.bin
        FILE *file = fopen(file_path, "wb");
        if (file == NULL) {
            perror("Error creating schedule.bin");
            return;
        }
        if (fwrite(personal_table->schedule, sizeof(short), TABLE_MAX_TIME * TABLE_MAX_DAY, file) != TABLE_MAX_TIME * TABLE_MAX_DAY) {
            perror("Error writing to schedule.bin");
            fclose(file);
            return;
        }
        fclose(file);
        #ifdef DEBUG
            printf("[DEBUG] schedule.bin created and initialized to 0.\n");
        #endif
    } else { // If file exists:
        // Open schedule.bin for reading
        FILE *file = fopen(file_path, "rb");
        if (file == NULL) {
            perror("Error opening schedule.bin");
            return;
        }
        if (fread(personal_table->schedule, sizeof(short), TABLE_MAX_TIME * TABLE_MAX_DAY, file) != TABLE_MAX_TIME * TABLE_MAX_DAY) {
            perror("Error reading from schedule.bin");
            fclose(file);
            return;
        }
        fclose(file);
        #ifdef DEBUG
            printf("[DEBUG] schedule.bin loaded successfully.\n");
        #endif
    }
}

void update_personal_table(Personal_Table *personal_table) {
    char file_path[MAX_USER_SCHEDULE_FILE_PATH];

    // Construct the file path: TEAM_BASE_DIR/team_name/username/schedule.bin
    snprintf(file_path, sizeof(file_path), "%s/%s/%s/schedule.bin", TEAM_BASE_DIR, personal_table->team_name, personal_table->username);

    // Open schedule.bin for writing (create or overwrite)
    FILE *file = fopen(file_path, "wb");
    if (file == NULL) {
        perror("Error opening schedule.bin for writing");
        return;
    }

    // Write data from personal_table->schedule to schedule.bin
    if (fwrite(personal_table->schedule, sizeof(short), TABLE_MAX_TIME * TABLE_MAX_DAY, file) != TABLE_MAX_TIME * TABLE_MAX_DAY) {
        perror("Error writing to schedule.bin");
        fclose(file);
        return;
    }

    fclose(file);
    #ifdef DEBUG
        printf("schedule.bin updated successfully from personal_table.schedule.\n");
    #endif
}