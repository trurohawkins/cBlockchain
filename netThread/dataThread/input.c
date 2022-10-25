#include "input.h"
#include "data.c"
bool runningMainThread = true;

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

int getInput(char *buffer, int letterCount) {
	if (letterCount == -1) {
		return -1;
	}
	nonblock(1);
	int i = 0;
	char c;
	int interval = 9999;
	int iCounter = 0;
	while(!i && iCounter < interval) {
		usleep(1);
		i=kbhit();
		if (i != 0) {
			c=fgetc(stdin);
			if (c== 27) {
				return -2;//kill sequence
			} else if (c==10) {
				i=1;
			} else {
				buffer[letterCount++] = c;
				if (letterCount < 1024) {
					i=0;
				} else {
					printf("\n");
					i=1;
				}
			}
		}
		iCounter++;
	}
	if (i == 1) {
		buffer[letterCount] = '\0';
		letterCount  = -1;
	}
	nonblock(0);
	return letterCount;
}

void *inputThread(void *buffer) {
	pthread_mutex_t *lock = calloc(1, sizeof(pthread_mutex_t));
	memcpy(lock, buffer, sizeof(pthread_mutex_t*));
	pthread_mutex_unlock(lock);

	char *inpBuffer = (char*)calloc(sizeof(char), BUFF + 1);
	int letterCount = 0;
	while(runningMainThread) {
		letterCount = getInput(inpBuffer, letterCount);
		if (letterCount == -1) {
			if (pthread_mutex_trylock(lock) == 0) {
				letterCount = 0;
				int len = strlen(inpBuffer) + 1;
				Data *d = makeData(inpBuffer, len);
				void *buff = writeData(d);
				memcpy(buffer, buff, len + sizeof(int));
				pthread_mutex_unlock(lock);
				
				memset(inpBuffer, 0, BUFF);
				free(buff);
				free(d);
			} else {
				printf("cannot lock mutex - input\n");
			}
			//pthread_mutex_unlock(lock);
		} else if (letterCount == -2) {
			runningMainThread = false;
			memset(inpBuffer, 0, BUFF);
		}
	}
	free(lock);
	free(inpBuffer);
	pthread_exit(0);
}


