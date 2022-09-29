//#include <stdbool.h>
//#include<stdio.h>
//#include <stdlib.h>
//#include <string.h>
#include "transaction.h"

int main(int argc, char* argv[]) {
	//genPrimes(1000, 50000);
	srand(time(0));
	Key *sender = makeKeyPair();
	Key *recipient = makeKeyPair();
	printKey(sender);
	printKey(recipient);
	NUM *cipher = signTransaction(sender, recipient[0], 10);
	Transaction *t = decryptTransaction(cipher, sender[0]);
	if (t) {
		printTransaction(t);
		free(t);
	}
	free(cipher);
	free(sender);
	free(recipient);
	return 0;
}

