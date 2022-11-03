#include "inputThread.h"
#include "data.c"
bool runningMainThread = true;

BuffLock *makeBuffLock() {
	BuffLock *bl = calloc(1, sizeof(BuffLock));
	bl->lock = calloc(1, sizeof(pthread_mutex_t));
	if (pthread_mutex_init(bl->lock, NULL) != 0) {
		printf("error in mutex init\n");
		return 0;
	}
	bl->buffer = calloc(1, BUFF);
	return bl;
}

void freeBuffLock(BuffLock *bl) {
	free(bl->buffer);
	free(bl->lock);
	free(bl);
}

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

pthread_t createThread(void*(*func)(void*), void *data, int state) {
	pthread_t threadHandle;
	// specifies properties of thread
	pthread_attr_t attr;
	pthread_attr_init(&attr);
	pthread_attr_setdetachstate(&attr, state
		/*PTHREAD_CREATE_JOINABLE
		PTHREAD_CREATE_DETACHED*/);

	// send int via heap memory not local variable
	/*
	void * _data = calloc(1, sizeof(int));
	*_data= data;
	*/
	//create fork point
	pthread_create(&threadHandle, &attr, func, data);
	pthread_attr_destroy(&attr);
	return threadHandle;
}

void *inputThread(void *buffer) {
	BuffLock *bl = buffer;
	//pthread_mutex_lock(bl->lock);

	char *inpBuffer = (char*)calloc(sizeof(char), BUFF + 1);
	int letterCount = 0;
	while(runningMainThread) {
		letterCount = getInput(inpBuffer, letterCount);
		if (letterCount == -1) {
			if (pthread_mutex_trylock(bl->lock) == 0) {
				letterCount = 0;
				int len = strlen(inpBuffer) + 1;
				Data *d = makeData(inpBuffer, len);
				void *buff = writeData(d);
				memcpy(bl->buffer, buff, len + sizeof(int));
				pthread_mutex_unlock(bl->lock);
				memset(inpBuffer, 0, BUFF);
				free(buff);
				freeData(d);
			}
		} else if (letterCount == -2) {
			runningMainThread = false;
			memset(inpBuffer, 0, BUFF);
		}
	}
	free(inpBuffer);
	pthread_exit(0);
}

