#include "server.c"

int main() {
	char *message = "Achknowledgement message, sent from server\n";
	welcomeSize = strlen(message) + 1;
	welcomeMessage = calloc(welcomeSize, sizeof(char));
	memcpy(welcomeMessage, message, welcomeSize);
	runServer(0);	
}
