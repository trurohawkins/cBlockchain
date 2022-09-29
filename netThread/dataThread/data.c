#include "data.h"

Data *makeData(void *arr, int bytes) {
	Data *d = (Data*)calloc(1, sizeof(Data));
	d->arr = arr;
	d->byteSize = bytes;
	return d;
}

void *writeData(Data *d) {
	void *buff = calloc(1, d->byteSize + sizeof(int));
	memcpy(buff, &d->byteSize, sizeof(int));
	memcpy(buff + sizeof(int), d->arr, d->byteSize);
	return buff;
}

Data *readData(void *buffer) {
	Data *d = (Data*)calloc(1, sizeof(Data));
	memcpy(&d->byteSize, buffer, sizeof(int));
	//printf("rread data of size %i\n", d->byteSize);
	d->arr = calloc(1, d->byteSize);
	memcpy(d->arr, buffer + sizeof(int), d->byteSize);
	return d;
}

void freeData(Data *d) {
	free(d->arr);
	free(d);
}

