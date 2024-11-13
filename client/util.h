#ifndef UTIL_H
#define UTIL_H

void tty_mode(int how);
void set_cr_noecho_mode();
void set_non_blocking_mode();
void show_title(int start_line, const char* title);

#endif
