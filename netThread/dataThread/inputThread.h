#ifndef INPUT 
#define INPUT

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include<stdbool.h>
#include <curses.h>
#include <termios.h>

#define BUFF 1024

#include "data.h"

typedef struct BuffLock {
	pthread_mutex_t *lock;
	void *buffer;
} BuffLock;

extern bool runningMainThread;

BuffLock *makeBuffLock();
void freeBuffLock(BuffLock *bl);

void nonblock(int state);
int kbhit();

void printBinary(void *buff, int bytes);
int getInput(char *buffer, int letterCount);

pthread_t createThread(void*(*func)(void*), void *data, int state);
void *inputThread(void *buffers);
#endif
