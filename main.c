#include "dataThread/keyboardFunc.c"
#include "dataThread/threads.c"
#include "dataThread/dataInput.c"
#include "network/client.c"
#include "network/server.c"

int main(int argc, char **argv) {
	char *ip = 0;
	if (argc > 1) {
		ip = argv[1];
	}
	/*
	char *message = "Achknowledgement message, sent from server\n";
	welcomeSize = strlen(message) + 1;
	welcomeMessage = calloc(welcomeSize, sizeof(char));
	memcpy(welcomeMessage, message, welcomeSize);
*/
	serverBuffer = (char*)calloc(sizeof(char), BUFF + 1);
	createThread(runServer, 0, PTHREAD_CREATE_DETACHED);
	clientBuffer = (char*)calloc(sizeof(char), BUFF + 1);
	if (ip != 0) { 
		createThread(runClient, (void*)ip, PTHREAD_CREATE_DETACHED);
	}
	void *buffs = calloc(sizeof(void*), 2);
	memcpy(buffs, &clientBuffer, sizeof(void*));
	memcpy(buffs + sizeof(void*), &serverBuffer, sizeof(void*));
	Data *b = makeData(buffs, sizeof(void*) * 2);
	void *buffbuff = writeData(b);
	createThread(inputThread, buffbuff, PTHREAD_CREATE_DETACHED);
	//inputThread();
	runningMainThread = true;

	while (runningMainThread) {
		if (runningServer && *(int*)serverBuffer != 0) {
			Data *read = readData(serverBuffer);
			printf("server got data2: %s\n", (char*)read->arr); 
			free(read->arr);
			free(read);
			memset(serverBuffer, 0, BUFF);//sizeof(inpBuffer));
		}
		if (runningClient && *(int*)clientBuffer != 0) {
			printf("pooopoo\n");
			Data *read = readData(clientBuffer);
			write(c->sock, read, BUFF);
			
			printf("client got data: %s\n", (char*)read->arr); 
			free(read->arr);
			free(read);
			memset(clientBuffer, 0, BUFF);//sizeof(inpBuffer));
		}
	}
	runningServer = false;
	runningClient = false;
	pthread_exit(0);

}
