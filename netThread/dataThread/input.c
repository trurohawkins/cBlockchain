#include "input.h"

bool runningMainThread = false;
bool runningInputThread = false;

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
				//printf("kil lsequence\n");
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

void *inputThread(void *buffer) {
	/*
	Data *buffs = readData(buffers);
	int buffCount = buffs->byteSize / sizeof(void*);
	void **buffer = buffs->arr;
	*/
	char *inpBuffer = (char*)calloc(sizeof(char), BUFF + 1);
	int letterCount = 0;
	runningInputThread = true;

	while(runningInputThread) {
		letterCount = getInput(inpBuffer, letterCount);
		if (letterCount == -1) {//inpBuffer[letterCount] == '\0') {
			letterCount = 0;
			//printf("string len: %i\n", strlen(inpBuffer));
			int len = strlen(inpBuffer) + 1;
			Data *d = makeData(inpBuffer, len);
			void *buff = writeData(d);
			/*
			for (int i = 0; i < buffCount; i++) {
				memcpy(buffer[i], buff, BUFF);
			}
			*/
			memcpy(buffer, buff, len + sizeof(int));
			memset(inpBuffer, 0, BUFF);//sizeof(inpBuffer));
			free(buff);
			free(d);
		} else if (letterCount == -2) {
			runningMainThread = false;
			runningInputThread = false;
			memset(inpBuffer, 0, BUFF);//sizeof(inpBuffer));
		}
	}
	free(inpBuffer);
	pthread_exit(0);
}


