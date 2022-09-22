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

#define PORT 6969
#define MAXCLIENTS 10
#define BUFF 1024

typedef struct Server {
	int sock;
	int maxClients;
	int *clientSocks;
	int addrlen;
	struct sockaddr_in addr;
	struct timeval tv;
} Server;
extern void *welcomeMessage;
extern int welcomeSize;
extern char *serverBuffer;
extern bool runningServer;
extern Server *s;

Server *makeServer(int maxClients);
void freeServer(Server *s);
void closeServer(Server *s);
Server *setUpServerConnection();
int serverSendReceive(Server *s, void *buffer);
void *runServer(void*);
