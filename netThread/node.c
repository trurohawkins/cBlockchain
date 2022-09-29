#include "node.h" 

bool networkStarted = false;
bool timeToStart = false;

void startNode(char *ip) {
	serverInpBuff = (char*)calloc(sizeof(char), BUFF + 1);
	serverDaisyBuff = (char*)calloc(sizeof(char), BUFF + 1);

	//createThread(runServer, 0, PTHREAD_CREATE_DETACHED);
	//clientInpBuff = (char*)calloc(sizeof(char), BUFF + 1);
	clientDaisyBuff = (char*)calloc(sizeof(char), BUFF + 1);
	int buffCount = 2;
	if (ip != 0) { 
		//createThread(runClient, (void*)ip, PTHREAD_CREATE_DETACHED);
	} else {
		//free(clientInpBuff);
		//free(clientDaisyBuff);
	}
	buffCount = 1;
	void *buffs = calloc(sizeof(void*), buffCount);
	memcpy(buffs, &serverInpBuff, sizeof(void*));
	if (buffCount > 1) {
		memcpy(buffs + sizeof(void*), &clientInpBuff, sizeof(void*));
	}
	Data *b = makeData(buffs, sizeof(void*) * buffCount);
	void *buffbuff = writeData(b);
	createThread(inputThread, buffbuff, PTHREAD_CREATE_DETACHED);
}

void runNode(void (*processData)(void*, bool), void (*welcome)(void), void (*parse)(char*, bool), char *ip) {
	runningMainThread = true;
	while (runningMainThread) {
		if (timeToStart && !networkStarted) {
			welcome();
			createThread(runServer, 0, PTHREAD_CREATE_DETACHED);
			if (ip) {
				createThread(runClient, (void*)ip, PTHREAD_CREATE_DETACHED);
			}
			networkStarted = true;
		} else {
			// buffer from input thread
			if (*(int*)serverInpBuff != 0) {
				parse(serverInpBuff, true);
				memset(serverInpBuff, 0, BUFF);//sizeof(inpBuffer));
			}
			if (runningServer) {
				/*
				if (*(int*)serverInpBuff != 0) {
					parse(serverInpBuff, true);
					memset(serverInpBuff, 0, BUFF);//sizeof(inpBuffer));
				}
				*/
				if (*(int*) serverDaisyBuff != 0) {
					//unpack data
					processData(serverDaisyBuff, true);
					memset(serverDaisyBuff, 0, BUFF);//sizeof(serverDaisyBuff));
				}
			}
			if (runningClient) {
				/*
				if (*(int*)clientInpBuff != 0) {
					parse(clientInpBuff, false);
					memset(clientInpBuff, 0, BUFF);//sizeof(inpBuffer));
				}
				*/
				// received from connected server
				if (*(int*)clientDaisyBuff != 0) {
					//uunpack data
					processData(clientDaisyBuff, false);
					memset(clientDaisyBuff, 0, BUFF);
				}
			}
		}
	}
	runningServer = false;
	runningClient = false;
	pthread_exit(0);
}

void sendInput(char *buff, bool onServer) {
	if (runningServer) {
		serverSendAll(s, buff, BUFF);
	}
	if (runningClient) {
		write(c->sock, buff, BUFF);
	}
}

void processString(void *buff, bool onServer) {
	if (onServer) {
		if (c != 0 && runningClient) {
			write(c->sock, serverDaisyBuff, BUFF);//strlen(serverBuffer));
		}
	} else {
		if (s != 0 && runningServer) {
			serverSendAll(s, clientDaisyBuff, BUFF); // strlen(clientBuffer));
		}
	}
	Data *read = readData(buff);
	printf("%s\n", (char*)read->arr); 
	freeData(read);
	/*
	free(read->arr);
	free(read);
	*/
}

void welcomeText() {
	char *message = "Achknowledgement message, sent from server\n";
	welcomeSize = strlen(message) + 3;
	void *welcomeBuff = calloc(welcomeSize, sizeof(char));
	memcpy(welcomeBuff, message, welcomeSize);
	Data *d = makeData(welcomeBuff, welcomeSize);
	welcomeMessage = writeData(d);
}
