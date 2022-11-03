#ifndef NODE
#define NODE

#include "network/client.h"
#include "network/server.h"
#include "dataThread/inputThread.h"

extern bool networkStarted;
extern bool timeToStart;
 
void runNode(void (*processData)(void*, bool), void (*welcome)(void), void (*parse)(char*), char *ip);
void *runServer(void *buff);
void *runClient(void *buff);

void processString(void*, bool onServer);
void sendInput(char *buff);
void welcomeText();

#endif
