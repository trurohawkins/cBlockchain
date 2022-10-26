#include "data.h"

bool littleEndian() {
	unsigned int i = 1;
	char *c = (char*)&i;
	if (*c) {
		return true;
	} else {
		return false;
	}
}

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
	int size = *(int*)buffer;
	if (size < BUFF - sizeof(int) && size > 0) {
		Data *d = (Data*)calloc(1, sizeof(Data));
		d->byteSize = size;
		d->arr = calloc(1, d->byteSize);
		memcpy(d->arr, buffer + sizeof(int), d->byteSize);
		return d;
	} else {
		printf("bad read byte size: %i,should be > 0 &&  < %i\n", size, BUFF - sizeof(int));
		return 0;
	}
}

void freeData(Data *d) {
	free(d->arr);
	free(d);
}
