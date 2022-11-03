#ifndef DATA
#define DATA
#include <arpa/inet.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

typedef struct Data {
	int bytes;
	void *arr;
} Data;

Data *makeData(void *arr, int bytes);
void *writeData(Data *d);
Data *readData(void *buffer);
void freeData(Data *d);
void printBinary(void *buff, int bytes);
void *flipEndian(void *buff, int bytes);
#endif
