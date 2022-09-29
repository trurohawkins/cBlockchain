#include "networkCommand.h"

bool takingName = true;

Data *myContactMessage() {
	Data *mes	= 0;
	if (mainUser) {
		Contact *c = getContact(mainUser);
		int size = contactLength(c);
		void *con = packContact(c);
		Data *d = makeData(con, size);
		mes = writeDataMessage(5, d);
		freeContact(c);
	}
	return mes;
}

void welcomeToBlockchain() {
	Data *welcomeData = myContactMessage();
	welcomeSize = welcomeData->byteSize;
	welcomeMessage = welcomeData->arr;
	printHelp();
}

void processBlockCommand(void *buffer, bool onServer) {
	Data *reply = parseDataMessage(buffer);
	if (reply) {
		if (onServer) {
			serverSendAll(s, reply->arr, reply->byteSize);
		} else {
			write(c->sock, reply->arr, reply->byteSize);
		}
		freeData(reply);
	}
} 

Data *writeDataMessage(int mes, Data *d) {
	int size = sizeof(int) + sizeof(int) + d->byteSize;
	void *buffer = calloc(1, size);
	memcpy(buffer, &mes, sizeof(int));
	memcpy(buffer + sizeof(int), &(d->byteSize), sizeof(int));
	memcpy(buffer + sizeof(int) + sizeof(int), d->arr, d->byteSize);
	Data *message = makeData(buffer, size);
	freeData(d);
	return message;
}

Data *parseDataMessage(void *buff) {
	int mes = 0;
	int size = 0;
	memcpy(&mes, buff, sizeof(int));
	if (mes < 0 || mes > 5) {
		//printf("got %i\n", mes);
		return 0;
	}
	memcpy(&size, buff + sizeof(int), sizeof(int));
	//printf("mes:%i and size: %i\n", mes, size);
	void *arr = calloc(1, size);
	memcpy(arr, buff + sizeof(int) + sizeof(int), size);
	Data *d = makeData(arr, size);
	//printf("parsing data message ");
	return parseData(mes, d);
}

/*\
 |	1 - new transaction
 |	2 - new block 
 		3 - chain check
 |	4 - new chain
 |  5 - new contact
\*/

Data *parseData(int mes, Data *d) {
	if (mes == 0) {
		return 0;
	}
	//printf("received %i\n", mes);
	if (mes == 1) {
		Key *k = (Key*)calloc(1, sizeof(Key));
		memcpy(k, d->arr, sizeof(Key));
		printf("got new transaction from ");
		printKey(k);
		if(verifyTransaction(d->arr + sizeof(Key), *k, myChain)) {
			printf("good transaction\n");
		} else {
			printf("bad transactiobn\n");
		}
		printf("\n");
		free(k);
		freeData(d);
	} else if (mes == 2) {
		Block *b = unpackBlockData(d->arr);
		freeData(d);
		if(addBlock(myChain,  b)) {
			printf("received good block\n\n");
			pruneClosedTransactions(myChain);
		}	else {
			printf("received bad block\n\n");
		}
		printPoolPack(b);
		
	} else if (mes == 3) {
		int incLen;
		memcpy(&incLen, d->arr, sizeof(int));
		//printf("received chain length %i\n", incLen);
		freeData(d);
		if (myChain) {
			if (myChain->len > incLen) {
				//return our chain
				//printf("ssending our chain\n");
				return blockchainMessage(myChain);
			} else if (myChain->len == incLen) {
				return 0;
			}
		}
		//request a chain
		return chainLengthMessage(myChain);
	} else if (mes == 4) {
		int incLen;
		memcpy(&incLen, d->arr, sizeof(int));
		if (myChain) {
			if (myChain->len >= incLen) {
				return 0;
			} else {
				freeBlockchain(myChain);
			}
		} else {
			printf("no chain here!!!\n");
		}
		//update our chain
		myChain = unpackChain(d->arr);
		freeData(d);
		printf("----GOT NEW CHAIN----\n");
		printChain(myChain, printPoolPack);
		//go through chain, find local open transactions and remove them
		pruneClosedTransactions(myChain);
	} else if (mes == 5) {
		Contact *c = unpackContact(d->arr);
		//printf("received contact %s\n", c->name);
		Data *reply = 0;
		if (addToContacts(c)) {
			printf("\n received new contact\n");
			printContact(c);
			printf("\n");
			reply = myContactMessage();
		} else {
			freeContact(c);
		}
		freeData(d);
		printf("\n");
		return reply;
	} else {
		if (d) {
			freeData(d);
		} else {
			//printf("no d\n");
		}
	}
	return 0;
}

Data *blockchainMessage(Blockchain *chain) {
	Data *d = packChain(chain);
	return writeDataMessage(4, d);
}


int strToInt(char **str) {
	//printf("converting -%s-\n", *str);
	int num = 0;
	int len = strlen(*str);
	float power = 0.1;
	for (int i = 0; i < len; i++) {
		if ((*str)[i] == 32) {

		} else if ((*str)[i] > 47 && (*str)[i] < 58) {
			power *= 10;
			//printf(" %c - %f", (*str)[i], power);
		} else {
			len = i;
			break;
		}
	}
	//printf("\nlen is %i and power is %f\n", len, power);
	for (int i = 0; i < len; i++) {
		if ((*str)[i] >= 48 && (*str)[i] < 58) {
			num += ((*str)[i] - 48) * power;
			power /= 10;
		}
	}
	*str += len;
	return num;
}

Data *chainLengthMessage(Blockchain *bc) {
	if (bc) {
		//printf("sending our length %i\n", bc->len);
		int *len = calloc(2, sizeof(int));
		*len = bc->len;
		Data *d  = makeData(len, sizeof(int));
		Data *mes = writeDataMessage(3, d);
		return mes;
	} else {
		return 0;
	}
}

void parseCommand(char *dat, bool) {
	Data *d = readData(dat);
	char *str = d->arr;
	printf("parsing %s\n", str);
	if (!timeToStart) {
		mainUser = genUser(str);
		saveUser("user.u", mainUser);
		timeToStart = true;
		printf("saved %s\n", str);
		printUser(mainUser);
		Contact *c = getContact(mainUser);
		addToContacts(c);
		printContacts();
		return;
	}
	Data *mes = 0;
	if (str[0] == 't' || str[0] == 'T') {
		//printf("command transaction %s\n", str);
		mes = newTransactionMessage(str);
	} if (str[0] == 'm' || str[0] == 'M') {
		//mine a block
		mes = newBlockMessage();
	} else if (str[0] == 'c' || str[0] == 'C') {
		mes = myContactMessage();
	} else if (str[0] == 'p' || str[0] == 'P') {
		printChain(myChain, printPoolPack);
	} else if (str[0] == 'h' || str[0] == 'H') {
		printHelp();
	} else if (str[0] == 'b' || str[0] == 'B') {
		printf("account balance for %s is %f\n\n", mainUser->name, accountBalance(mainUser->publicKey, myChain));
	}
	if (mes) {
		if (runningClient) {
			write(c->sock, mes->arr, mes->byteSize);
		}
		if (runningServer) {
			serverSendAll(s, mes->arr, mes->byteSize);
		}
		freeData(mes);
	}
	
}

Data *newBlockMessage() {
	Contact *miner = getContact(mainUser);
	Block *b = packPool(myChain, miner);
	Data *mes = 0;
	freeContact(miner);
	if (b) {
		printf("mining block\n");
		if (addBlock(myChain, b)) {
			printf("successfully mined a block\n-----\n");
			printPoolPack(myChain->cur);
			int size = blockSize(b);
			void *buff = packBlockData(b);
			Data *block = makeData(buff, size);
			mes = writeDataMessage(2, block);
		}
	} else {
		printf("nothing to mine\n");
	}
	return mes;
}

Data *newTransactionMessage(char *com) {
	char *num = com + 2;
	Data *mes = 0;
	//int amount = strToInt(&num);
	float amount = atof(num);
	//move passed number
	while (*num != 32) {
		if (*num != 0) {
			num++;
		} else {
			break;
		}
	}
	//move thru space
	while (*num == 32) {
		if (*num != 0) {
			num++;
		} else {
			break;
		}
	}
	Contact *c = findContactName(num);
	if (c) {
		NUM *tHash = genTransaction(mainUser, c, amount);
		Contact *sender = getContact(mainUser);
		if (verifyTransaction(tHash, sender->publicKey, myChain)) {
			Transaction *display = makeTransaction(&(mainUser->publicKey), &(c->publicKey), amount);
			printf("transaction pending...\n");
			printTransaction(display);
			free(display);
			//24 for transaction, 2 for sender cotnact public key
			int size = sizeof(NUM) * 32;
			void *tBuff = calloc(1, size);

			memcpy(tBuff, &(sender->publicKey), sizeof(Key));
			memcpy(tBuff + sizeof(Key), tHash, sizeof(NUM) * 24);

			Data *d = makeData(tBuff, sizeof(NUM) * 32);
			mes = writeDataMessage(1, d);
		} else {
			printf("invalid transaction generated\n");
		}
		//free(buff);
		free(tHash);
		freeContact(sender);
	} else {
		printf("couldnt find %s\n", num);
	}
	return mes;
}

void printHelp() {
	printf("\n-------Blockchain Command-------\n");
	printf("to use, type in command and press enter\npress esc to quit\n");
	printf("-------commands-------\n");
	printf("T <amount> <contactName>  --\n");
	printf("   sends <amount> of coin to <contactName>\n");
	printf("M -----\n");
	printf("   begins the mining process and then broadcasts the new block\n");
	printf("P -----\n");
	printf("   prints out your blockchain\n");
	printf("B -----\n");
	printf("   prints out your account balance\n");
	printf("H -----\n");
	printf("   displays this help message again\n");
	printf("--------------------------------\n\n");
}

