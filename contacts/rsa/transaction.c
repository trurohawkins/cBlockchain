#include "transaction.h"

NUM *signTransaction(Key *sender, Key recipient, float amount) {
	NUM bigAmount = 0;
	memcpy(&bigAmount, &amount, sizeof(float));
	NUM data[6] = {sender[0].n, sender[0].c, bigAmount, recipient.n, recipient.c, (NUM)time(NULL)};
	NUM pack[24];
	int count = 0;
	for (int i = 0; i < 6; i++) {
		NUM *smalls = splitBigNum(&(data[i]));
		for (int j = 0; j < 4; j++) {
			pack[count] = smalls[j];
			count++;
		}
		free(smalls);
	}
	NUM *cipher = cryptData(pack, 24, sender[1]);
	return cipher;
}

Transaction *makeTransaction(Key *sender, Key *recipient, float amount) {
	Transaction *t = (Transaction*)calloc(1, sizeof(Transaction));
	// for initial source transaction
	if (sender) {
		t->sender = *sender;
	}
	t->recipient = *recipient;
	t->amount = amount;
	t->timestamp = time(NULL);
	return t;
}

Transaction *decryptTransaction(NUM *transaction, Key sender) {
	NUM *mes = cryptData(transaction, 24, sender);
	if (!mes){
		printf("bad key for decryption %"PRId64" and %"PRId64"\n", sender.c, sender.n);
		return 0;
	}
	for (int i = 0; i < 24; i++) {
		//printf("OG: %"PRId64" \nCR: %"PRId64"\n\n", pack[i], mes[i]);
	}
	//Key *k = (Key*)calloc(1, sizeof(Key));
	NUM data2[6];
	int count = 0;
	for (int i = 0; i < 6; i++) {
		NUM smalls[4];
		for (int j = 0; j < 4; j++) {
			smalls[j] = mes[count];
			count++;
		}
		data2[i] = combineSmallNums(smalls);
		//printf("%"PRId64" ", data2[i]);
	}
	//printf("\n");
	Transaction *t = (Transaction*)calloc(1, sizeof(Transaction));
	t->sender.n = data2[0];
	t->sender.c = data2[1];
	memcpy(&(t->amount), data2 + 2, sizeof(float));
	///t->amount = data2[2];
	t->recipient.n = data2[3];
	t->recipient.c = data2[4];
	t->timestamp = data2[5];
	free(mes);
	if (compareKeys(sender, t->sender)) {
		return t;
	} else {
		printKey(&sender);
		printKey(&t->sender);
		free(t);
		return 0;
	}
	//check for getBalance(k) > data[2]
	
}

NUM *splitBigNum(NUM *n) {
	//printBinary(n, sizeof(NUM));
	NUM *smalls = (NUM*)calloc(4, sizeof(NUM));
	for (int i = 0; i < 4; i++) {
		NUM big = (*n) << sizeof(NUM) * (2*i);
		big = (uint64_t)big >> sizeof(NUM) * 6;
		smalls[i] = big;
		//printBinary(&(smalls[i]), sizeof(NUM));
	}
	return smalls;
}

NUM combineSmallNums(NUM *n) {
	NUM big = 0;
	for (int i = 0; i < 4; i++) {
		NUM tmp = n[i] << sizeof(NUM) * (6 - (i * 2));
		big |= tmp;
	}
	return big;
}

bool compareTransactions(Transaction *t1, Transaction *t2) {
	if (t1->timestamp == t2->timestamp) {
		if (t1->amount == t2->amount) {
			if (compareKeys(t1->sender, t2->sender) && compareKeys(t1->recipient, t2->recipient)) {
				return true;	
			}
		}
	}
	return false;
}

void printTransaction(Transaction *t) {
	printf("%"PRId64" is sending %f to %"PRId64" at %i\n", t->sender.n, t->amount, t->recipient.n, t->timestamp);
}

