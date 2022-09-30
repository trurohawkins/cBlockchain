#include "networkCommand.h"

int main(int argc, char **argv) {
	srand(time(0));
	initContacts();
	/*
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
*/
	myChain = loadChain("blockchain.sav");
	if (!myChain) {
		myChain = makeBlockchain();
	}
	char *ip = 0;
	if (argc > 1) {
		ip = argv[1];
	}
	/*
	char *message = "Achknowledgement message, sent from server\n";
	welcomeSize = strlen(message) + 3;
	void *welcomeBuff = calloc(welcomeSize, sizeof(char));
	memcpy(welcomeBuff, message, welcomeSize);
	Data *d = makeData(welcomeBuff, welcomeSize);
	welcomeMessage = writeData(d);
	*/

	startNode(ip);
	mainUser = loadUser("user.u");
	if (!mainUser) {
		timeToStart = false;
		printf("no user.u found\nCreating new user please enter a name and press enter\n");
	} else {
		printf("welcome back %s\n\n", mainUser->name);
		timeToStart = true;
		
	}
	runNode(processBlockCommand, welcomeToBlockchain, parseCommand, ip);
	free(welcomeMessage);
	freeUser(mainUser);
	freePools();
	freeContacts();
	if (validateChain(myChain)) {
		saveChain("blockchain.sav", myChain);
	}
	freeBlockchain(myChain);
}
