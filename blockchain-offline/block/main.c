#include "block.h"

void printTransBlock(Block *b) {
	printf("prev: ");
	printHash(b->preHash);
	printf("data: ");
	printHash(b->dataHash);
	if (b->data != NULL) {
		printTransaction(b->data->arr);
	}
	printf("proof: %i\n", b->proof);
	printf("timestamp: %i\n", b->timestamp);
	printf("-------------------\n");

}

int main() {
	srand(time(0));
	initContacts();
	User *u1 = loadUser("Crunch.u");
	if (!u1) {
		u1 = genUser("Crunch");
		saveUser("Crunch.u", u1);
	}
	User *u2 = loadUser("Bono.u");
	if (!u2) {
		u2 = genUser("Bono");
		saveUser("Bono.u", u2);
	}
	Contact *c1 = getContact(u1);
	Contact *c2 = getContact(u2);

	addToContacts(c1);
	addToContacts(c2);
	freeContact(c1);
	freeContact(c2);
	
	Blockchain *chain = loadChain("blockchain.sav");
	if (!chain) {
		chain = makeBlockchain();
	}
	Block *tamper;
	int chainSize = 5;

	for (int i = 0; i < chainSize; i++) {
		NUM *c = genTransaction(u1, findContactName("Bono"), i);
		Transaction *t = decryptTransaction(c, u1->publicKey);
		free(c);
		Data *data = makeData(t, sizeof(Transaction));
		/*
		int *data = calloc(1, sizeof(int));
		*data = i;
		Data *d = makeData(data, sizeof(int));
		*/
		Block *b = makeBlock(data, chain->cur);
		addBlock(chain, b);
		if (i == chainSize / 2) {
			tamper = b;
		}
	}
	
	printChain(chain, printTransBlock);
	printf("is this chain valid? %s\n", validateChain(chain) ? "true" : "false");
	saveChain("blockchain.sav", chain);
	
	Data *d = tamper->data;
	Transaction *t = d->arr;
	t->amount = 100;
	//*((int*)(d->arr)) = 100;
	printf("\n");	
	printChain(chain, printTransBlock);
	printf("is this chain valid? %s\n", validateChain(chain) ? "true" : "false");
	printf("\n");
	freeUser(u1);
	freeUser(u2);
	freeContacts();
	freeBlockchain(chain);
	return 0;
}
