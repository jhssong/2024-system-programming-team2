#ifndef UTIL_H
#define UTIL_H

extern int window_width;
extern int window_height;

void get_window_size();

void tty_mode(int how);
void set_cr_noecho_mode();
void set_non_blocking_mode();
void show_title(const char* title);

#endif
