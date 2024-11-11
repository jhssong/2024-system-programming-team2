#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <termios.h>
#include <fcntl.h>

#define NUMBER_OF_CHAR_AT_A_TIME 1

void tty_mode(int how) {
	static struct termios orig_mode;

	if (how == 0) 					// Save current setting
		tcgetattr(0, &orig_mode); 
	else if (how == 1)				// Recover saved setting
		tcsetattr(0, TCSANOW, &orig_mode);
}

void set_cr_noecho_mode() {
	struct termios ttyinfo;
	
	if (tcgetattr(0, &ttyinfo) == -1) {
		perror("tcgetattr");
		exit(EXIT_FAILURE);
	}

	ttyinfo.c_lflag &= ~ICANON;		// Set non-canonical mode
	ttyinfo.c_lflag &= ~ECHO;		// Disable ECHO bit
	ttyinfo.c_cc[VMIN] = NUMBER_OF_CHAR_AT_A_TIME;

	if (tcsetattr(0, TCSANOW, &ttyinfo) == -1) {
		perror("tcsetattr");
		exit(EXIT_FAILURE);
	}
}

void set_non_blocking_mode() {
    int terflags;
    terflags = fcntl(0, F_GETFL);
    terflags |= O_NONBLOCK;
    fcntl(0, F_SETFL, terflags);
}
