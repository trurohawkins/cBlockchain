#include "client.c"

int main(int argc, char **argv) {
	if (argc < 2) {
		printf("please provide an ip to connect to\n");
		return 1;
	} else {
		clientBuffer = (char*)calloc(sizeof(char), BUFF + 1);
		runClient(argv[1]);
	}
}
