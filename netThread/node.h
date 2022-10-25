#ifndef NODE
#define NODE

#include "network/client.h"
#include "network/server.h"
#include "dataThread/input.h"
#include "dataThread/data.h"

extern bool networkStarted;
extern bool timeToStart;

void startNode(char *ip);
void runNode(void (*processData)(void*, bool), void (*welcome)(void), void (*parse)(char*,bool), char *ip);
void processString(void*, bool);
void sendInput(char *buff, bool onServer);
void welcomeText();

#endif
