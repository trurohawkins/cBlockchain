#include "transactionPool.h"

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
	int poolSize = 9;
	for (int i = 0; i < poolSize; i++) {
		NUM *c = genTransaction(u1, findContactName("Bono"), 0);
		if (!verifyTransaction(c, u1->publicKey, chain)) {
			printf("couldn't verify transaction\n");
			return 1;
		}
		sleep(1);
	}
	printPool();
	int chainSize = 3;
	for (int i = 0; i < chainSize; i++) {
		Block *b = packPool(chain, findContactName("Crunch"));
		if (!addBlock(chain, b)) {
			printf("couldn't add block\n");
			return 1;
		}
	}
	printChain(chain, printPoolPack);
	printf("accopunt balance of Crunch %f\n", accountBalance(u1->publicKey, chain));
	if (validateChain(chain)) {
		saveChain("blockchain.sav", chain);
	}
	return 0;
}
