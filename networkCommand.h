#include "blockchain-offline/transactionPool.h"
#include "netThread/node.h"

void processBlockCommand(void *buffer, bool onServer);
void welcomeToBlockchain();
Data *writeDataMessage(int mes, Data *d);
Data *parseDataMessage(void *buff);
Data *parseData(int mes, Data *d);
void processBlockCommand(void *buffer, bool onServer);
void parseCommand(char *str, bool);
int strToInt(char **str);
Data *chainLengthMessage(Blockchain *bc);
Data *blockchainMessage(Blockchain *chain);
Data *myContactMessage();
Data *newBlockMessage();
Data *newTransactionMessage(char *com);
void printHelp();
