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

extern bool runningMainThread;

void nonblock(int state);
int kbhit();

void printBinary(void *buff, int bytes);
int getInput(char *buffer, int letterCount);

#endif
