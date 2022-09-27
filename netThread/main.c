#include "dataThread/keyboardFunc.c"
#include "dataThread/threads.c"
#include "dataThread/dataInput.c"
#include "network/client.c"
#include "network/server.c"
#include "netThread.c"

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
	startNode(ip);
	//inputThread();
	runNode();
}
