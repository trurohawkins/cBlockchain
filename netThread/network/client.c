#include "client.h"

char *clientInpBuff;
char *clientDaisyBuff;
bool runningClient = false;
Client *c;

Client *connectAsClient(char *ip) {
	Client *c = (Client*)calloc(sizeof(Client), 1);
	c->sock = 0;
	c->tv.tv_sec = 1;
	c->tv.tv_usec = 0;
	struct sockaddr_in clientAddr;
	fd_set fdset;
	if ((c->sock = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
		perror("client socket failed");
		return 0;
	}
	clientAddr.sin_addr.s_addr = inet_addr(ip);
	clientAddr.sin_family = AF_INET;
	clientAddr.sin_port = htons(PORT);

	fcntl(c->sock, F_SETFL, O_NONBLOCK);

	connect(c->sock, (const struct sockaddr*)&clientAddr, sizeof(clientAddr));
	FD_ZERO(&fdset);
	FD_SET(c->sock, &fdset);
	int activity = select(c->sock + 1, NULL, &fdset, NULL, &(c->tv));
	if (activity == 1) {
		int so_error;
		socklen_t len = sizeof(so_error);
		getsockopt(c->sock, SOL_SOCKET, SO_ERROR, &so_error, &len);
		if (so_error == 0) {
			printf("~~~Client Connected -- %s:%d is Open~~~\n", ip, PORT);
		} else {
			printf("client connection failed\n");
			close(c->sock);
			free(c);
			return 0;
		}
	} else {
		printf("client connection failed %d\n", activity);
		perror(ip);
		close(c->sock);
		free(c);
		return 0;
	}
	return c;
}

int receiveData(Client *c, void *recBuff) {
	fd_set readfds;
	int valread = 0;
	FD_ZERO(&readfds);
	FD_SET(c->sock, &readfds);
	select(c->sock + 1, &readfds, NULL, NULL, &(c->tv));
	/*
	if (strlen(recBuff) > 0) {
		printf("I am receiving data to %s\n", recBuff);
	}
	*/
	if (FD_ISSET(c->sock, &readfds)) {
		if ((valread = read(c->sock, recBuff, BUFF)) == 0) {
			printf("no more server, gonna close client socket\n");
			return -1;
		}
	}
	return valread;
}

void *runClient(void *buff) {
	pthread_t *lock = calloc(1, sizeof(pthread_mutex_t));
	memcpy(lock, buff, sizeof(pthread_mutex_t));
	pthread_mutex_unlock(lock);
	//c = connectAsClient(buff + sizeof(pthread_mutex_t));
	//runningClient = c > 0;
	if (runningClient) {
		//clientDaisyBuff = (char *)calloc(sizeof(char), BUFF + 1);
		char *buffer = (char *)calloc(sizeof(char), BUFF + 1);
		int val = 0;
		while(runningClient) {
			if (val == 0) {
				val = receiveData(c, buffer); 
			} else {
				if (val < 0) {
					runningClient = false;
				} else if (val > 0) {
					if (pthread_mutex_trylock(lock) == 0) {
						memcpy(buff, buffer, BUFF);
						pthread_mutex_unlock(lock);
						memset(buffer, 0, BUFF);
						val = 0;
					}
				}
			}
		}
		free(buffer);
		//free(clientDaisyBuff);
		close(c->sock);
		free(c);
	}
}
