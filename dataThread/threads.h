#include <pthread.h>

void createThread(void*(*func)(void*), void *data, int state);
void runFuncThread(void*(*func)(void*));
