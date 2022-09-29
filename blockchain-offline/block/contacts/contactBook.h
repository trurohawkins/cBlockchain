#include "rsa/transaction.h"

typedef struct Contact {
	char *name;
	Key publicKey;
} Contact;

typedef struct User {
	char *name;
	Key publicKey;
	Key privateKey;
} User;

typedef struct ContactPage {
	Contact *data;
	struct ContactPage *next;
} ContactPage;

typedef struct ContactBook {
	ContactPage *book[27];
	int byteSize;
} ContactBook;

extern ContactBook* contacts;
extern User *mainUser;

User *genUser(char *name);
void saveUser(char *file, User *u);
void freeUser(User *u);
User *loadUser(char *file);
Contact *getContact(User *u);
Contact *makeContact(char *name, Key key);
NUM *genTransaction(User *u, Contact *c, float amount);

void *packContact(Contact *c);
Contact *unpackContact(void *buffer);

void initContacts();
bool addToContacts(Contact *c);
void freeContacts();
void printContacts();

Contact *findContact(Key publicKey);
Contact *findContactName(char *name);

void saveContacts();
void loadContacts();

void freeContact(Contact *c);
int contactLength(Contact *c);
void printUser(User *u);
void printContact(Contact *c);
