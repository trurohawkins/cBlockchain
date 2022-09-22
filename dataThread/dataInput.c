#include "dataInput.h"

bool runningMainThread = false;

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
	printf("rread data of size %i\n", d->byteSize);
	d->arr = calloc(1, d->byteSize);
	memcpy(d->arr, buffer + sizeof(int), d->byteSize);
	return d;
}


int getInput(char *buffer, int letterCount) {
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

void *inputThread(void *buffers) {
	Data *buffs = readData(buffers);
	int buffCount = buffs->byteSize / sizeof(void*);
	printf("got %i buffs\n", buffCount);
	void **buffer = buffs->arr;
	printf("i-buffer:%p\n", *buffer);
	char *inpBuffer = (char*)calloc(sizeof(char), BUFF + 1);
	int letterCount = 0;
	while(runningMainThread) {
		letterCount = getInput(inpBuffer, letterCount);
		if (letterCount == -1) {//inpBuffer[letterCount] == '\0') {
			letterCount = 0;
			Data *d = makeData(inpBuffer, BUFF);
			void *buff = writeData(d);
			for (int i = 0; i < buffCount; i++) {
				memcpy(buffer[i], buff, BUFF);
			}
			memset(inpBuffer, 0, BUFF);//sizeof(inpBuffer));
		} else if (letterCount == -2) {
			runningMainThread = false;
			memset(inpBuffer, 0, BUFF);//sizeof(inpBuffer));
		}
		// daisy chain all servers to pass on message to node network
		/*
		if (strlen(serverBuffer) > 0) {
			if (c != 0 && runningClient) {
				write(c->sock, serverBuffer, strlen(serverBuffer));
			}
			memset(serverBuffer, 0, sizeof(serverBuffer));
		}
		if (strlen(clientBuffer) > 0) {
			if (s != 0 && runningServer) {
				serverSendAll(s, clientBuffer, strlen(clientBuffer));
			}
			memset(clientBuffer, 0, sizeof(clientBuffer));
		}
		*/
	}
	free(inpBuffer);
}


