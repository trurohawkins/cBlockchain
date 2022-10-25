#ifndef INPUT 
#define INPUT

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include<stdbool.h>
#include <curses.h>
#include <termios.h>

#define BUFF 1024
#include"threads.h"

extern bool runningMainThread;

void nonblock(int state);
int kbhit();

void printBinary(void *buff, int bytes);
int getInput(char *buffer, int letterCount);
void *inputThread(void *buffers);

#endif
