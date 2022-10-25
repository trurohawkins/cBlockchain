#ifndef THREAD
#define THREAD
#include <pthread.h>

//extern pthread_mutex_t *lock;

pthread_t createThread(void*(*func)(void*), void *data, int state);
void runFuncThread(void*(*func)(void*));
#endif
