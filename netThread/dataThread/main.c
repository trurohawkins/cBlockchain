#include "input.h"

int main() {
	int *poo = (int*)calloc(1, sizeof(int));
	*poo = 10;
	Data *d = makeData(poo, sizeof(int));
	void *buffer = writeData(d);
	freeData(d);
	Data *read = readData(buffer);
	free(buffer);
	//printf("%i\n", *(int*)read->arr);
	freeData(read);

	void *inputBuffer = calloc(sizeof(char), BUFF + 1);
	/*
	printf("i-buffer:%p\n", inputBuffer);
	void *inputBuffer2 = calloc(sizeof(char), BUFF + 1);
	void *buffs = calloc(sizeof(void*), 2);
	memcpy(buffs, &inputBuffer, sizeof(void*));
	memcpy(buffs + sizeof(void*), &inputBuffer2, sizeof(void*));
	Data *b = makeData(buffs, sizeof(void*) * 2);
	void *buffbuff = writeData(b);
	*/
	createThread(inputThread, inputBuffer, PTHREAD_CREATE_DETACHED);
	runningMainThread = true;

	while (runningMainThread) {
		if (*(int*)inputBuffer != 0) {
			Data *read = readData(inputBuffer);
			printf("got data: %s\n", (char*)read->arr); 
			freeData(read);
			memset(inputBuffer, 0, BUFF);//sizeof(inpBuffer));
		}
		/*
		if (*(int*)inputBuffer != 0) {
			Data *read = readData(inputBuffer);
			printf("got data: %s\n", (char*)read->arr); 
			free(read);
			memset(inputBuffer, 0, BUFF);//sizeof(inpBuffer));
		}
		*/
	}
	free(inputBuffer);
	pthread_exit(0);
}
