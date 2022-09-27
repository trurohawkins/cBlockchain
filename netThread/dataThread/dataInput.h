#define BUFF 1024
#include "keyboardFunc.h"
#include "threads.h"

extern bool runningMainThread;

typedef struct Data {
	int byteSize;
	void *arr;
} Data;

Data *makeData(void *arr, int bytes);
void *writeData(Data *d);
Data *readData(void *buffer);

int getInput(char *buffer, int letterCount);
void *inputThread(void *buffers);
