#include <stdbool.h>
#include<stdio.h>
#include <stdlib.h>
#include <string.h>

#include "rsa/rsaCrypt.c"
#include "rsa/transaction.c"
#include "contactBook.c"

int main(int argc, char **argv) {
	initContacts();
	User *u1 = loadUser("Cunch.u");
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

	printContacts();

	NUM *c = genTransaction(u1, findContactName("Bono"), 0);
	Transaction *t = decryptTransaction(c, u1->publicKey);
	free(c);
	printTransaction(t);
	free(t);

	saveContacts();
	freeContacts();

	freeUser(u1);
	freeUser(u2);
}
