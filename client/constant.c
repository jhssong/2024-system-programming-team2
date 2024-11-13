#include "constant.h"

int window_width = 0;
int window_height = 0;

void get_window_size() {
	struct winsize wbuf;

	if (ioctl(0, TIOCGWINSZ, &wbuf) != -1) {
		window_width = wbuf.ws_col;
		window_height = wbuf.ws_row;
	}
}

