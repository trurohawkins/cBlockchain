#include "node.h" 

void startNode(char *ip) {
	serverInpBuff = (char*)calloc(sizeof(char), BUFF + 1);
	serverDaisyBuff = (char*)calloc(sizeof(char), BUFF + 1);
	createThread(runServer, 0, PTHREAD_CREATE_DETACHED);
	clientInpBuff = (char*)calloc(sizeof(char), BUFF + 1);
	clientDaisyBuff = (char*)calloc(sizeof(char), BUFF + 1);
	int buffCount = 2;
	if (ip != 0) { 
		createThread(runClient, (void*)ip, PTHREAD_CREATE_DETACHED);
	} else {
		free(clientInpBuff);
		free(clientDaisyBuff);
		buffCount = 1;
	}
	void *buffs = calloc(sizeof(void*), buffCount);
	memcpy(buffs, &serverInpBuff, sizeof(void*));
	if (buffCount > 1) {
		memcpy(buffs + sizeof(void*), &clientInpBuff, sizeof(void*));
	}
	Data *b = makeData(buffs, sizeof(void*) * buffCount);
	void *buffbuff = writeData(b);
	createThread(inputThread, buffbuff, PTHREAD_CREATE_DETACHED);
}

void runNode() {
	runningMainThread = true;

	while (runningMainThread) {
		if (runningServer) {
			if (*(int*)serverInpBuff != 0) {
				serverSendAll(s, serverInpBuff, BUFF);
				/*
				Data *read = readData(serverInpBuff);
				printf("server got data2: %s\n", (char*)read->arr); 
				free(read->arr);
				free(read);
				*/
				memset(serverInpBuff, 0, BUFF);//sizeof(inpBuffer));
			}
			if (*(int*) serverDaisyBuff != 0) {
				if (c != 0 && runningClient) {
					write(c->sock, serverDaisyBuff, BUFF);//strlen(serverBuffer));
				}
				//unpack data
				memset(serverDaisyBuff, 0, BUFF);//sizeof(serverDaisyBuff));
			}
		}
		if (runningClient) {
			if (*(int*)clientInpBuff != 0) {
				printf("pooopoo\n");
				write(c->sock, clientInpBuff, BUFF);
				/*
				Data *read = readData(clientInpBuff);
				
				printf("client got data: %s\n", (char*)read->arr); 
				free(read->arr);
				free(read);
				*/
				memset(clientInpBuff, 0, BUFF);//sizeof(inpBuffer));
			}
			if (*(int*)clientDaisyBuff != 0) {
				if (s != 0 && runningServer) {
					serverSendAll(s, clientDaisyBuff, BUFF); // strlen(clientBuffer));
				}
				//uunpack data
				memset(clientDaisyBuff, 0, BUFF);
			}
		}
	}
	runningServer = false;
	runningClient = false;
	pthread_exit(0);

}
