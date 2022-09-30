#include "node.c"

int main(int argc, char **argv) {
	char *ip = 0;
	if (argc > 1) {
		ip = argv[1];
	}

	startNode(ip);
	timeToStart = true;
	runNode(processString, welcomeText, sendInput, ip);
	free(welcomeMessage);
	pthread_exit(0);
}
