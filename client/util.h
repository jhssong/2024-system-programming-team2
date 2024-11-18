#ifndef UTIL_H
#define UTIL_H

extern int current_line;

// void tty_mode(int how);
// void set_cr_noecho_mode();
// void set_non_blocking_mode();
void show_title(const char* title);
int select_item(const char* item_list[], int list_size);

#endif
