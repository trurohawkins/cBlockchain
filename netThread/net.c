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
		free(lock);
		close(c->sock);
		free(c);
	}
}

void *runServer(void *buff) {
	pthread_t *lock = calloc(1, sizeof(pthread_mutex_t));
	memcpy(lock, buff, sizeof(pthread_mutex_t));
	pthread_mutex_unlock(lock);
	//s = setUpServerConnection();
	//runningServer = s > 0;
	//if (runningServer) {
		//serverDaisyBuff = (char*)calloc(sizeof(char), BUFF + 1);
		char *buffer = (char *)calloc(sizeof(char), BUFF + 1);
		int val = 0;
		while (runningServer) {
			val = serverSendReceive(s, buffer, val); 
			if (val > 0) {
				if (pthread_mutex_trylock(lock) == 0) {
					memcpy(buff, buffer, val);
					pthread_mutex_unlock(lock);

					memset(buffer, 0, BUFF);
					val = 0;
				}
			} else {
				//printf("got val: %i\n", val);
			}
		}
		printf("server ended\n");
		free(buffer);
		free(lock);
		closeServer(s);
	//}
}

