#include "node.h" 

bool timeToStart = false;

void startNode(char *ip) {
	//serverDaisyBuff = (char*)calloc(sizeof(char), BUFF + 1);

	//createThread(runServer, 0, PTHREAD_CREATE_DETACHED);
	//clientInpBuff = (char*)calloc(sizeof(char), BUFF + 1);
	//clientDaisyBuff = (char*)calloc(sizeof(char), BUFF + 1);
	/*
	int buffCount = 2;
	if (ip != 0) { 
		//createThread(runClient, (void*)ip, PTHREAD_CREATE_DETACHED);
	} else {
		//free(clientInpBuff);
		//free(clientDaisyBuff);
	}
	buffCount = 1;
	void *buffs = calloc(sizeof(void*), buffCount);
	memcpy(buffs, &inputBuffer, sizeof(void*));
	if (buffCount > 1) {
		memcpy(buffs + sizeof(void*), &clientInpBuff, sizeof(void*));
	}
	Data *b = makeData(buffs, sizeof(void*) * buffCount);
	void *buffbuff = writeData(b);
	*/
}

void runNode(void (*processData)(void*, bool), void (*welcome)(void), void (*parse)(char*, bool), char *ip) {
	runningMainThread = true;
	void *inputBuffer = (char*)calloc(sizeof(char), BUFF + 1);
	serverDaisyBuff = (char*)calloc(sizeof(char), BUFF + 1);
	clientDaisyBuff = (char*)calloc(sizeof(char), BUFF + 1);
	pthread_t handles[3] = {0,0,0};
	
	pthread_mutex_t *lock = calloc(sizeof(pthread_mutex_t), 1);
	if (pthread_mutex_init(lock, NULL) != 0) {
		printf("mutex initialization failed\n");
		return 1;
	}
	pthread_mutex_t *s_lock = calloc(sizeof(pthread_mutex_t), 1);
	if (pthread_mutex_init(s_lock, NULL) != 0) {
		printf("mutex initialization failed\n");
		return 1;
	}
	pthread_mutex_t *c_lock = calloc(sizeof(pthread_mutex_t), 1);
	if (pthread_mutex_init(c_lock, NULL) != 0) {
		printf("mutex initialization failed\n");
		return 1;
	}
	
	memcpy(serverDaisyBuff, &s_lock, sizeof(pthread_mutex_t));
	memcpy(clientDaisyBuff, &c_lock, sizeof(pthread_mutex_t));
	memcpy(inputBuffer, &lock, sizeof(pthread_mutex_t));

	handles[0] = createThread(inputThread, inputBuffer, PTHREAD_CREATE_DETACHED);
	memset(inputBuffer, 0 , BUFF);
	while (runningMainThread) {
		if (timeToStart && !(runningClient || runningServer)) {
			welcome();
			if (handles[1] == 0) {
				handles[1] = createThread(runServer, serverDaisyBuff, PTHREAD_CREATE_DETACHED);
			} else {
				memset(serverDaisyBuff, 0, BUFF);
			}
			if (handles[2] == 0) {
				if (ip) {
					c = connectAsClient(ip);
					if (c > 0) {
						handles[2] = createThread(runClient, clientDaisyBuff, PTHREAD_CREATE_DETACHED);
						runningClient = true;
					}
				}
			} else {
				memset(clientDaisyBuff, 0, BUFF);
			}
		} else {
			if (pthread_mutex_trylock(lock) == 0) {
				// buffer from input thread
				if (*(int*)inputBuffer != 0) {
					parse(inputBuffer, true);
					memset(inputBuffer, 0, BUFF);//sizeof(inpBuffer));
				}
				pthread_mutex_unlock(lock);
			}
			if (runningServer) {
				if (pthread_mutex_trylock(s_lock) == 0) {
					if (*(int*) serverDaisyBuff != 0) {
						//unpack data
						processData(serverDaisyBuff, true);
						memset(serverDaisyBuff, 0, BUFF);//sizeof(serverDaisyBuff));
					}
					pthread_mutex_unlock(s_lock);
				}
			}
			if (runningClient) {
				if (pthread_mutex_trylock(c_lock) == 0) {
					// received from connected server
					if (*(int*)clientDaisyBuff != 0) {
						//uunpack data
						processData(clientDaisyBuff, false);
						memset(clientDaisyBuff, 0, BUFF);
					}
					pthread_mutex_unlock(c_lock);
				}
			}
			if (!runningClient && !runningServer) {
				runningMainThread = false;
			}
		}
	}
	printf("main thead ended %i\n", runningServer);
	runningServer = false;
	runningClient = false;
	for (int i = 0; i < 3; i++) {
		if (handles[i] != 0) {
			pthread_join(handles[i], 0);
		}
	}
	free(inputBuffer);
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
	if (read) {
		printf("%s\n", (char*)read->arr); 
		freeData(read);
	}
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
	freeData(d);
}
