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
	//inputThread();
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
			if (!(int*)clientDaisyBuff != 0) {
				if (s != 0 && runningServer) {
					serverSendAll(s, clientDaisyBuff, BUFF); // strlen(clientBuffer));
				}
				memset(clientDaisyBuff, 0, BUFF);
			}
		}
		/*
		// daisy chain all servers to pass on message to node network
		if (strlen(s) > 0) {
			if (c != 0 && runningClient) {
				write(c->sock, s, strlen(serverBuffer));
			}
			memset(s, 0, sizeof(serverBuffer));
		}
		if (strlen(clientBuffer) > 0) {
			if (s != 0 && runningServer) {
				serverSendAll(s, clientBuffer, strlen(clientBuffer));
			}
			memset(clientBuffer, 0, sizeof(clientBuffer));
		}
		*/
	}
	runningServer = false;
	runningClient = false;
	pthread_exit(0);

}
