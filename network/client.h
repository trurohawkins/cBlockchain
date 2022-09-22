#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <stdbool.h>
#include <sys/types.h>
#include <fcntl.h>

#define PORT 6969
#define MAXCLIENTS 10
#define BUFF 1024
extern char *clientBuffer;
extern bool runningClient;

typedef struct Client {
	int sock;
	struct timeval tv;
} Client;
extern Client *c;

Client *connectAsClient(char *ip);
int receiveData(Client *c, void *recBuff);
void *runClient(void *ip);
