void *runClient(void *buff) {
	/*
	pthread_mutex_t *lock = calloc(1, sizeof(pthread_mutex_t));
	memcpy(lock, buff, sizeof(pthread_mutex_t));
	pthread_mutex_unlock(lock);
	*/
	BuffLock *bulo = buff;
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
					if (pthread_mutex_trylock(bulo->lock) == 0) {
						memcpy(bulo->buffer, buffer, BUFF);
						pthread_mutex_unlock(bulo->lock);
						memset(buffer, 0, BUFF);
						val = 0;
					}
				}
			}
		}
		close(c->sock);
		free(c);
		free(buffer);
	}
}

void *runServer(void *buff) {
	BuffLock *bulo = buff;
	/*
	pthread_mutex_t *lock = calloc(1, sizeof(pthread_mutex_t));
	memcpy(lock, buff, sizeof(pthread_mutex_t));
	pthread_mutex_unlock(lock);
	*/
	//s = setUpServerConnection();
	//runningServer = s > 0;
	//if (runningServer) {
		//serverDaisyBuff = (char*)calloc(sizeof(char), BUFF + 1);
		char *buffer = (char *)calloc(sizeof(char), BUFF + 1);
		int val = 0;
		while (runningServer) {
			serverSendReceive(s, buffer, &val); 
			if (val > 0) {
				if (pthread_mutex_trylock(bulo->lock) == 0) {
					memcpy(bulo->buffer, buffer, val);
					pthread_mutex_unlock(bulo->lock);

					memset(buffer, 0, BUFF);
					val = 0;
				}
			} else {
				//printf("got val: %i\n", val);
			}
		}
		printf("server ended\n");
		free(welcomeMessage);
		free(buffer);
		closeServer(s);
	//}
}

