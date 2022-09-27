#include "TransactionPool.h"

Pool *poolList;
Pool *readyTransactions;

Blockchain *initBlockchain() {
	Blockchain *bc = makeBlockchain();

}

bool verifyTransaction(NUM *t, Key *sender, Blockchain *poolChain) {
	Transaction *trans = decryptTransaction(t, *sender);
	if (trans) {
		//Contact *sender = findContact(conBook);
		//Contact *recipient = findContact(trans->recipient, conBook);
		//if (recipient) {
			//check get_balance(t->sender) > t->amount;
			if (accountBalance(trans->sender, poolChain) >= trans->amount) {
				if (!searchForTransaction(trans)) {
					addToPool(trans);
					//printPool();
				}
				//addToPool(trans);
			} else {
				free(trans);
				//freeContact(recipient);
				printf("insufficient funds\n");
				return false;
			}
			//freeContact(recipient);
			return true;
		/*} else {
			printf("no recipient\n");
		}*/
	} else {
		printf("couldnt decrypt\n");
	}
	return false;
}

bool searchForTransaction(Transaction *t) {
	Pool *cur = poolList;
	while (cur) {
		for (int i = 0; i < cur->poolCount; i++) {
			if (compareTransactions(t, &(cur->transactions[i]))) {
				return true;
			}
		}
		cur = cur->next;
	}
	cur = readyTransactions;
	while (cur) {
		for (int i = 0; i < cur->poolCount; i++) {
			if (compareTransactions(t, &(cur->transactions[i]))) {
				return true;
			}
		}
		cur = cur->next;
	}
	return false;
}


void addToPool(Transaction *t) {
	Pool *cur = poolList;
	Pool *pre = poolList;
	while (cur) {
		if (cur->poolCount < POOLSIZE) {
			break;
		} 
		pre = cur;
		cur = cur->next;
	}
	if (!cur) {
		cur = (Pool*)calloc(1, sizeof(Pool));
		if (pre) {
			pre->next = cur;
		} else {
			poolList = cur;
		}
	}
	(cur->transactions)[cur->poolCount] = *t;
	free(t);
	cur->poolCount++;
	if (cur->poolCount >= POOLSIZE) {
		if (pre == cur) {
			poolList = cur->next;
		} else {
			pre->next = cur->next;
		}
		//add free to ready
		if (!readyTransactions) {
			readyTransactions = cur;//(Pool*)calloc(1, sizeof(Pool));
			//*readyTransactions = *cur;
		} else {
			pre = readyTransactions;
			while (pre->next) {
				pre = pre->next;
			}
			pre->next = cur;
			//free(cur);
		}
	}
}

bool removeFromTransactions(Transaction *t) {
	Pool *cur = poolList;
	Pool *pre = poolList;
	while (cur) {
		if (cur->poolCount > 0) {
			for (int i = 0; i < cur->poolCount; i++) {
				if (compareTransactions(t, &(cur->transactions[i]))) {
					for (int j = i; j < cur->poolCount - 1; j++) {
						cur->transactions[j] = cur->transactions[j + 1];
					}
					cur->poolCount--;
					return true;
				}
			}
		}
		cur = cur->next;
	}
	cur = readyTransactions;
	pre = readyTransactions;
	while (cur) {
		if (cur->poolCount > 0) {
			for (int i = 0; i < cur->poolCount; i++) {
				if (compareTransactions(t, &(cur->transactions[i]))) {
					//remove
					for (int j = i; j < cur->poolCount - 1; j++) {
						cur->transactions[j] = cur->transactions[j + 1];
					}
					cur->poolCount--;
					//free(tmp);
					return true;
				}
			}
		}
		cur = cur->next;
	}
	//printf("transactio not here\n");
	return false;
}

void pruneClosedTransactions(Blockchain *bc) {
	//printf("\n pruning closed transactions \n");
	// go through nodes
	Block *cur = bc->base->next;
	while (cur) {
		// go through transactions in data
			Transaction *t = cur->data->arr + sizeof(int);
			for (int i = 0; i < (cur->data->byteSize - sizeof(int)) / sizeof(Transaction); i++) {
				if(removeFromTransactions(&(t[i]))) {
					//printf("removed\n");
				}
			}
		cur = cur->next;
	}
}


void printPool() {
	Pool *cur = poolList;
	int count = 0;
	printf("transactions being made\n");
	while (cur) {
		printf("pool#%i\n", count);
		if (cur->poolCount > 0) {
			for (int i = 0; i < cur->poolCount; i++) {
				printTransaction(&(cur->transactions[i]));
				printf("\n");
			}
			printf("\n");
		}
		count++;
		cur = cur->next;
	}
	count = 0;
	printf("ready pool\n");
	cur = readyTransactions;
	while (cur) {
		printf("pool#%i\n", count);
		if (cur->poolCount > 0) {
			for (int i = 0; i < cur->poolCount; i++) {
				printTransaction(&(cur->transactions[i]));
				printf("\n");
			}
			printf("\n");
		}
		count++;
		cur = cur->next;
	}
}

void freePools() {
	Pool *cur = poolList;
	while (cur) {
		Pool *next = cur->next;
		free(cur);	
		cur = next;
	}
	cur = readyTransactions;
	while (cur) {
		Pool *next = cur->next;
		free(cur);	
		cur = next;
	}
}

Pool *pop(Pool** p) {
	Pool *cur = *p;
	if (cur) {
		*p = (*p)->next;
	}
	return cur;
}

Block *packPool(Blockchain *bc, Contact *miner) {
	Pool *ready = pop(&readyTransactions);
	if (!ready) {
		ready = pop(&poolList);
	}
	if (!ready) {
		return 0;
	}
	int count = ready->poolCount + 1;
	void *buff = calloc(1, count * sizeof(Transaction) + sizeof(int));
	memcpy(buff, &(count), sizeof(int));
	printf("pool count %i, in buff %i\n",ready->poolCount,  buff);//ready->poolCount);
	Transaction *t = buff + sizeof(int);
	for (int i = 0; i < count - 1; i++) {
		((Transaction*)t)[i] = ready->transactions[i];
	}
	Transaction *reward  = makeTransaction(0, &(miner->publicKey), 1);
	memcpy(((Transaction*)t) + count - 1, reward, sizeof(Transaction));
	free(reward);
	Data *d = makeData(buff, count * sizeof(Transaction) + sizeof(int));
	Block *b = makeBlock(d, bc->cur);
	free(ready);	
	return b;
}

void printPoolPack(Block *b) {
	printf("prev: ");
	printHash(b->preHash);
	printf("data: ");
	printHash(b->dataHash);
	if (b->data) {
		int len;
		memcpy(&len, b->data->arr, sizeof(int));
		Transaction *t = b->data->arr + sizeof(int);
		for (int i = 0; i < len; i++) {
			printTransaction(&(t[i]));
		}
	} else {
		printf("\n");
	}
	printf("proof: %i\n", b->proof);
	printf("timestamp: %i\n", b->timestamp);
	printf("-------------------\n");
}

float poolBalance(Key account, Pool* p) {
	float earned = 0;
	float spent = 0;
	Pool *cur = p;
	while (cur) {
		for (int i = 0; i < cur->poolCount; i++) {
			Transaction *t = cur->transactions;
			if (compareKeys(t[i].recipient, account)) {
				earned += t[i].amount;
			}
			if (compareKeys(t[i].sender, account)) {
				spent += t[i].amount;
			}
		}
		cur = cur->next;
	}
	return earned - spent;
}


float accountBalance(Key account, Blockchain *bc) {
	float earned = poolBalance(account, poolList) + poolBalance(account, readyTransactions);
	float spent = 0;
	Block *b = bc->base;
	while (b) {
		if (b->data) {
			int len;
			memcpy(&len, b->data->arr, sizeof(int));
			Transaction *t = b->data->arr + sizeof(int);
			for (int i = 0; i < len; i++) {
				//printTransaction(t + i);	
				if (compareKeys(t[i].recipient, account)) {
					earned += t[i].amount;
				}
				if (compareKeys(t[i].sender, account)) {
					spent += t[i].amount;
				}
			}
		}
		b = b->next;
	}
	return earned - spent;
}


