#ifndef TRANSACTIONPOOL
#define TRANSACTIONPOOL
#include "block/block.h"

#define POOLSIZE 3

typedef struct Pool {
	Transaction transactions[POOLSIZE];
	int poolCount;
	struct Pool *next;
} Pool;

Blockchain *initBlockchain();
bool verifyTransaction(NUM *t, Key k, Blockchain *poolChain);
void addToPool(Transaction *t);
bool searchForTransaction(Transaction *t);
void pruneClosedTransactions(Blockchain *bc);

void freePools();
Block *packPool();
void printPool();
void printPoolPack (Block *);

float accountBalance(Key account, Blockchain *bc);
float poolBalance(Key account, Pool* p);
#endif
