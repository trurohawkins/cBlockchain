#ifndef INPUT 
#define INPUT

#define BUFF 1024
#include "keyboardFunc.h"
#include "threads.h"
#include "data.h"

extern bool runningMainThread;

int getInput(char *buffer, int letterCount);
void *inputThread(void *buffers);

#endif
