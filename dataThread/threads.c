#include "threads.h"

void createThread(void*(*func)(void*), void *data, int state) {
	pthread_t threadHandle;
	// specifies properties of thread
	pthread_attr_t attr;
	pthread_attr_init(&attr);
	pthread_attr_setdetachstate(&attr, state
		/*PTHREAD_CREATE_JOINABLE
		PTHREAD_CREATE_DETACHED*/);

	// send int via heap memory not local variable
	/*
	void * _data = calloc(1, sizeof(int));
	*_data= data;
	*/
	//create fork point
	pthread_create(&threadHandle, &attr, func, data);
	pthread_attr_destroy(&attr);
}

void runFuncThread(void*(*func)(void*)) {
	func(0);
	pthread_exit(0);
}

