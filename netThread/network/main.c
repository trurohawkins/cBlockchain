#include "server.c"

int main() {
	int send = 0;
	welcomeSize = sizeof(int);
	welcomeMessage = calloc(welcomeSize, 1);
	memcpy(welcomeMessage, &send, welcomeSize);

	s = setUpServerConnection();
	int count = 0;
	int interval = 1000000;
	if (s > 0) {
		bool runningServer = true;
		//serverDaisyBuff = (char*)calloc(sizeof(char), BUFF + 1);
		char *buffer = (char *)calloc(sizeof(char), BUFF + 1);
		while (runningServer) {
			int val = serverSendReceive(s, buffer); 
			if (val != 0) {
				printf("received %i\n", *(int*)buffer);
				//send = *(int*)buffer + 1;
				memset(buffer, 0, BUFF);
			} 
			if (count >= interval) {
				//serverSendAll(s, &send, sizeof(int));
				count = 0;
			} else {
				count++;
			}
		}
		printf("server ended\n");
		free(buffer);
		//free(serverDaisyBuff);
		closeServer(s);
	}
	//runServer(0);	
}
