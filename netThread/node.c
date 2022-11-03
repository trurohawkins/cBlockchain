#include "node.h" 

bool timeToStart = false;

void runNode(void (*processData)(void*, bool), void (*welcome)(void), void (*parse)(char*), char *ip) {
	timeToStart = true;
	runningMainThread = true;
	pthread_t handles[3] = {0,0,0};
	
	BuffLock *inpBulo = makeBuffLock();
	BuffLock *servBulo = makeBuffLock();
	BuffLock *cliBulo = makeBuffLock();

	handles[0] = createThread(inputThread, inpBulo, PTHREAD_CREATE_DETACHED);
	bool networkStarted = false;
	while (runningMainThread) {
		if (timeToStart && !networkStarted) {
			if (handles[1] == 0) {
				s = setUpServerConnection();
				if (s > 0) {
					welcome();
					handles[1] = createThread(runServer, servBulo, PTHREAD_CREATE_DETACHED);
					runningServer = true;
				}
			} else {
			}
			if (handles[2] == 0) {
				if (ip) {
					c = connectAsClient(ip);
					if (c > 0) {
						handles[2] = createThread(runClient, cliBulo, PTHREAD_CREATE_DETACHED);
						runningClient = true;
					}
				}
			} else {
			}
			networkStarted = true;
		} else {
			if (pthread_mutex_trylock(inpBulo->lock) == 0) {
				// buffer from input thread
				if (*(int*)inpBulo->buffer != 0) {
					parse(inpBulo->buffer);
					memset(inpBulo->buffer, 0, BUFF);//sizeof(inpBuffer));
				}
				pthread_mutex_unlock(inpBulo->lock);
			}
			if (runningServer) {
				if (pthread_mutex_trylock(servBulo->lock) == 0) {
					if (*(int*) servBulo->buffer != 0) {
						//unpack data
						processData(servBulo->buffer, true);
						if (c != 0 && runningClient) {
							write(c->sock, servBulo->buffer, BUFF);//strlen(serverBuffer));
						}
						memset(servBulo->buffer, 0, BUFF);//sizeof(serverDaisyBuff));
					}
					pthread_mutex_unlock(servBulo->lock);
				}
			}
			if (runningClient) {
				if (pthread_mutex_trylock(cliBulo->lock) == 0) {
					// received from connected server
					if (*(int*)cliBulo->buffer != 0) {
						//uunpack data
						processData(cliBulo->buffer, false);
						if (s != 0 && runningServer) {
							serverSendAll(s, cliBulo->buffer, BUFF); // strlen(clientBuffer));
						}
						memset(cliBulo->buffer, 0, BUFF);
					}
					pthread_mutex_unlock(cliBulo->lock);
				}
			}
			if (networkStarted && !runningClient && !runningServer) {
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
	freeBuffLock(inpBulo);
	freeBuffLock(servBulo);
	freeBuffLock(cliBulo);
}

void sendInput(char *buff) {
	if (runningServer) {
		serverSendAll(s, buff, BUFF);
	}
	if (runningClient) {
		write(c->sock, buff, BUFF);
	}
}

void processString(void *buff, bool onServer) {
	Data *read = readData(buff);
	if (read) {
		if (onServer) {
			printf("[client] %s\n", (char*)read->arr); 
		} else {
			printf("[server] %s\n", (char*)read->arr);
		}
		freeData(read);
	}
}

void welcomeText() {
	char *message = "Achknowledgement message, sent from server\n";
	welcomeSize = strlen(message) + 1;
	Data *wData = makeData(message, welcomeSize);
	welcomeSize += sizeof(int);
	welcomeMessage = writeData(wData);
	freeData(wData);
	/*
	void *welcomeBuff = calloc(welcomeSize, sizeof(char));
	memcpy(welcomeBuff, message, welcomeSize);
	Data *d = makeData(welcomeBuff, welcomeSize);
	welcomeMessage = writeData(d);
	free(welcomeBuff);
	freeData(d);
	*/
	//welcomeMessage = makeData(string, welcomeSize);
}

#include "net.c"
