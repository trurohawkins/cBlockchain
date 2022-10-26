#include "rsaCrypt.h"

typedef struct Transaction {
	Key sender;
	Key recipient;
	float amount;
	time_t timestamp;
} Transaction;


Transaction *makeTransaction(Key *sender, Key *recipient, float amount);
NUM *signTransaction(Key *sender, Key recipient, float amount);
Transaction *decryptTransaction(NUM *transaction, Key sender);
NUM *splitBigNum(NUM *n);
NUM combineSmallNums(NUM *n);
bool compareTransactions(Transaction *t1, Transaction *t2);
void printTransaction(Transaction *t);
