#include "server.h"
void *welcomeMessage;
int welcomeSize;
char *serverInpBuff;
char *serverDaisyBuff;
bool runningServer = false;

Server *s;

Server *makeServer(int maxClients) {
	Server *s = (Server *)calloc(sizeof(Server), 1);
	s->maxClients = maxClients;
	s->clientSocks = (int*)calloc(sizeof(int), maxClients);
	s->tv.tv_sec = 1;
	s->tv.tv_usec = 0;
	return s;
}

void freeServer(Server *s) {
	free(s->clientSocks);
	free(s);
}

void closeServer(Server *s) {
	//FD_ZERO(&readfds);
	for (int i = 0; i < s->maxClients; i++) {
		if (s->clientSocks[i] != 0) {
			printf("client %i still connected\n", s->clientSocks[i]);
			close(s->clientSocks[i]);
		}
	}
	close(s->sock);
	//printf("server socket closed\n");
	freeServer(s);
}

Server *setUpServerConnection() {
	Server *s = makeServer(MAXCLIENTS);
	int opt = 1;
	//initalise all client_socket[] to 0 so not checked
	if ((s->sock = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
		freeServer(s);
		perror("server socket failed");
		return 0;
	}
	//set master socket to allow multiple connectgions
	// this is just a good habit, it will work w/o
	if (setsockopt(s->sock, SOL_SOCKET, SO_REUSEADDR, (char *)&opt, sizeof(opt)) < 0) {
		freeServer(s);
		perror("setsockopt");
		return 0;
	}
	s->addr.sin_addr.s_addr = INADDR_ANY;
	s->addr.sin_family = AF_INET;
	s->addr.sin_port = htons(PORT);


	//bind socket to localhost port 
	if (bind(s->sock, (struct sockaddr *)&s->addr, sizeof(s->addr)) < 0) {
		freeServer(s);
		perror("server bind failed");
		printf("\n");
		return 0;
	}

	//Allow maximum of MAXCLIENTS pending connections
	if (listen(s->sock, MAXCLIENTS) < 0) {
		freeServer(s);
		perror("listen");
		//exit(EXIT_FAILURE);
		return 0;
	}
	s->addrlen = sizeof(s->addr);
	printf("~~Server up and Running~~\n");
	return s;
}

int serverSendReceive(Server *s, void *buffer) {
	//printf("----server----\n");
	int clientSock, sd, activity, gotData = 0, valread = 0;
	fd_set readfds, writefds;
	//char buffer [BUFF+1];
	//clear the socket set
	FD_ZERO(&readfds);
	FD_ZERO(&writefds);

	//add master socket to the set
	FD_SET(s->sock, &readfds);
	int maxSd = s->sock;
	//add child sockets to the set
	for (int i = 0; i < s->maxClients; i++) {
		sd = s->clientSocks[i];
		if (sd > 0) {
			//printf("adding %i\n", sd);
			FD_SET(sd, &readfds);
			FD_SET(sd, &writefds);
		}
		//update highest file descripotr for select function
		if (sd > maxSd) {
			maxSd = sd;
		}
	}
	//wait for activity on one of the sockets
	activity = select(maxSd + 1, &readfds, NULL, NULL, &(s->tv));

	if ((activity < 0) && (errno != EINTR)) {
		printf("select error\n");
	}
	//check if master socket(server) has info to be read
	if (FD_ISSET(s->sock, &readfds)) {//if send receive on client socket
		if ((clientSock = accept(s->sock, (struct sockaddr *)&(s->addr), (socklen_t*)&(s->addrlen))) < 0) {
			perror("accept");
			exit(EXIT_FAILURE);
		}

		//inform uuser of socket number - used in send and receive commands
		printf("New connection, socket fd %d, ip is: %s, port: %d\n", clientSock, inet_ntoa(s->addr.sin_addr), ntohs(s->addr.sin_port));
		//send new connection acknowledgement message
		if (send(clientSock, welcomeMessage, welcomeSize, 0) != (long int)welcomeSize) {
			perror("send");
		}
		printf("Acknowledgement message sent successfully from server\n\n");

		//add to new socket to array of sockets
		for (int i = 0; i < s->maxClients; i++) {
			//check if position is empty
			if (s->clientSocks[i] == 0) {
				s->clientSocks[i] = clientSock;
				break;
			}
		}
	}
	//else its some IO operations on some other sockets
	for (int i = 0; i < s->maxClients; i++) {
		sd = s->clientSocks[i];
		if (FD_ISSET(sd, &readfds)) { // if send receive on client socket
			//Check if it was for closing, and also read the incoming message
			if ((valread = read(sd, buffer, BUFF)) == 0) {
				close(sd);
				s->clientSocks[i] = 0;
				printf("lost connection to socket #%d\n", i);
			} else {
				//printf("got data\n");
				gotData = valread;
				for (int j = 1; j < s->maxClients; j++) {
					int cur = s->clientSocks[(i + j) % s->maxClients];
					if (cur != 0) {
						send(cur, buffer, valread, 0);
					}
				}
			}
		}
	}
	return gotData;
}

void serverSendAll(Server *s, void *buffer, int bytes) {
	for (int i = 0; i < s->maxClients; i++) {
		int cur = s->clientSocks[i];
		if (cur != 0) {
			send(cur, buffer, bytes, 0);
		}
	}
}

void *runServer(void *n) {
	s = setUpServerConnection();
	runningServer = s > 0;
	if (runningServer) {
		serverDaisyBuff = (char*)calloc(sizeof(char), BUFF + 1);
		char *buffer = (char *)calloc(sizeof(char), BUFF + 1);
		while (runningServer) {
			int val = serverSendReceive(s, buffer); 
			if (val != 0) {
				//printf("fudge %s\n", buffer);
				memcpy(serverDaisyBuff, buffer, BUFF);
				memset(buffer, 0, BUFF);
			}
		}
		printf("server ended\n");
		free(buffer);
		free(serverDaisyBuff);
		closeServer(s);
	}

}
