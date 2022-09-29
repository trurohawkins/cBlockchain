#ifndef DATA
#define DATA
#include <string.h>
#include <stdlib.h>


typedef struct Data {
	int byteSize;
	void *arr;
} Data;

Data *makeData(void *arr, int bytes);
void *writeData(Data *d);
Data *readData(void *buffer);
void freeData(Data *d);

#endif
