#include "keyboardFunc.h"

void nonblock(int state) {
	struct termios ttystate;
	//get the terminal state
	tcgetattr(STDIN_FILENO, &ttystate);
	if (state == 1) {
		//turn off canonical mode
		ttystate.c_lflag &= ~ICANON;
		//minimum number of input read
		ttystate.c_cc[VMIN] = 1;
	} else if (state == 0) {
		//turn on canonical mode
		ttystate.c_lflag |= ICANON;
	}
	//set the terminal attributes
	tcsetattr(STDIN_FILENO, TCSANOW, &ttystate);
}

int kbhit() {
	struct timeval tv;
	fd_set fds;
	tv.tv_sec = 0;
	tv.tv_usec = 0;
	FD_ZERO(&fds);
	FD_SET(STDIN_FILENO, &fds);
	select(STDIN_FILENO+1, &fds, NULL, NULL, &tv);
	return FD_ISSET(STDIN_FILENO, &fds);
}

