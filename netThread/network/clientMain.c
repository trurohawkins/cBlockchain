#include "client.c"

int main(int argc, char **argv) {
	if (argc < 2) {
		printf("please provide an ip to connect to\n");
		return 1;
	} else {
		runClient(argv[1]);
	}
}
