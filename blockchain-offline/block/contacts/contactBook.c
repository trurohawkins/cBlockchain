#include "contactBook.h"

ContactBook *contacts;
User *mainUser;

User *genUser(char *name) {
	/*
	NUM p = randomPrime();
	NUM q = randomPrime();
	while (p == q) {
		q = randomPrime();	
	}
	printf("generated %"PRIu64" and %"PRIu64" and together %"PRIu64"\n", p, q, p*q);
	*/
	Key *k = makeKeyPair();//genKeys(p, q);
	User *u = (User*)calloc(1, sizeof(User));
	u->publicKey = k[0];
	u->privateKey = k[1];
	u->name = (char*)calloc(strlen(name) + 1, sizeof(char));
	memcpy(u->name, name, strlen(name) + 1);
	free(k);
	return u;
}

void freeUser(User *u) {
	free(u->name);
	free(u);
}

void saveUser(char *file, User *u) {
	FILE *fptr = fopen(file, "wb");
	if (fptr != NULL) {
		int len = strlen(u->name) + 1;
		int size = (sizeof(Key) * 2) + len + sizeof(int);
		void *buff = calloc(1, size);
		memcpy(buff, &len, sizeof(int));
		memcpy(buff + sizeof(int), u->name, len);
		void *tmp = packKey(buff + sizeof(int) + len, &(u->publicKey));
		tmp = packKey(tmp, &(u->privateKey));
		tmp += sizeof(int);
		fwrite(buff, size, 1, fptr);
		free(buff);
		//printf("saved %i\n", size);
		fclose(fptr);
	}
}

User *loadUser(char *file) {
	User *u = 0;
	FILE *fptr = fopen(file, "rb");
	if (fptr != NULL) {
		printf("opened file %s\n", file);
		int len = 0;
		fread(&len, sizeof(int), 1, fptr);
		int size = len + (sizeof(Key) * 2);// + sizeof(int);
		void *buff = calloc(1, size);
		void *tmp = buff;
		int bytesRead = fread(buff, size, 1, fptr);
		if (bytesRead == 1) {
			u = (User *)calloc(1, sizeof(User));
			u->name = (char*)calloc(len, sizeof(char));
			memcpy(u->name, tmp, len);
			tmp += len;
			Key *k = unpackKey(tmp);
			u->publicKey = *k;
			free(k);
			tmp += sizeof(Key);
			k = unpackKey(tmp);
			u->privateKey = *k;
			free(k);
		} else {
			printf("loading user bytes size error opnly read %i, instead of %i\n", bytesRead, size);
		}
		free(buff);
		fclose(fptr);
	}
	return u;
}

NUM *genTransaction(User *u, Contact *c, float amount) {
	Key *keys = (Key*)calloc(2, sizeof(Key));
	keys[0] = u->publicKey;
	keys[1] = u->privateKey;
	NUM *trans = signTransaction(keys, c->publicKey, amount);
	free(keys);
	return trans;
}

void *packContact(Contact *c) {
	int len = strlen(c->name) + 1;
	int size = sizeof(int) + len + sizeof(Key);
	void *buff = calloc(1, size);
	void *tmp = packKey(buff, &(c->publicKey));
	memcpy(tmp, &len, sizeof(int));
	memcpy(tmp + sizeof(int), c->name, len);
	return buff;
}

Contact *unpackContact(void *buff) {
	Contact *c = (Contact*)malloc(1 * sizeof(Contact));
	Key *k = unpackKey(buff);
	c->publicKey = *k;
	free(k);
	buff += sizeof(Key);
	int len;
	memcpy(&len, buff, sizeof(int));
	c->name = (char*)calloc(len, sizeof(char));
	memcpy(c->name, buff + sizeof(int), len);
	return c;
}

Contact *makeContact(char *name, Key keys) {
	Contact *c = (Contact*)calloc(1, sizeof(Contact));
	int len = strlen(name) + 1;
	c->name = (char*)calloc(len, sizeof(char));
	memcpy(c->name, name, len);
	c->publicKey = keys;
	return c;
}
Contact *getContact(User *u) {
	Contact *c = makeContact(u->name, u->publicKey);//(Contact*)calloc(1, sizeof(Contact));
	return c;
}

void initContacts() {
	contacts = (ContactBook*)calloc(1, sizeof(ContactBook));
	for (int i = 0; i < 27; i++) {
		contacts->book[i] = 0;
	}
	contacts->byteSize = 0;
	loadContacts();
}

int charToIndex(char c) {
	char alph = c;
	if (alph >= 65 && alph <= 90) {
		alph -= 65;	
	} else if (alph >= 97 && alph <= 122) {
		alph -= 97;
	} else {
		alph = 26;
	}
	return alph;
}

bool addToContacts(Contact *c) {
	int alph = charToIndex(c->name[0]);
	ContactPage **cur = &(contacts->book[alph]);
	ContactPage **pre = cur;
	while (*cur) {
		if (!compareKeys((*cur)->data->publicKey, c->publicKey)) {
			pre = cur;
			cur = &((*cur)->next);
		} else {
			//printf("contact for %s is already here\n", c->name);
			return false;
		}
	}
	*cur = (ContactPage*)calloc(1, sizeof(ContactPage));
	(*cur)->data = makeContact(c->name, c->publicKey);//calloc(1, sizeof(Contact));
	//memcpy((*cur)->data, c, sizeof(Contact));
	//*(*cur)->data = *c;
	contacts->byteSize += contactLength(c);
	if (pre && pre != cur) {
		(*pre)->next = *cur;
	}
	//printf("added contact %s\n", c->name);
	return true;
}

Contact *findContactName(char *name) {
	int alph = charToIndex(*name);
	//printf("searching for %s\n", name);
	ContactPage *cur = contacts->book[alph];
	while (cur) {
		if (cur->data) {
			Contact *c = cur->data;//unpackContact(cur->data->arr);
			if (strcmp(name, c->name) == 0) {//;compareKeys(c->publicKey, publicKey)) {
				return c;
			}
		}
		cur = cur->next;
	}
	return 0;
}


Contact *findContact(Key publicKey) {
	for (int i = 0; i < 27; i++) {	
		ContactPage *cur = contacts->book[i];
		while (cur) {
			if (cur->data) {
				Contact *c = cur->data;//unpackContact(cur->data->arr);
				if (compareKeys(c->publicKey, publicKey)) {
					return c;
				}
			}
			cur = cur->next;
		}
	}
}

void freeContacts() {
	for (int i = 0; i < 27; i++) {
		if (contacts->book[i]) {
			ContactPage *cur = contacts->book[i];
			ContactPage *pre = cur;
			while (cur) {
				pre = cur->next;
				freeContact(cur->data);
				free(cur);
				cur = pre;
			}
		}
	}
	free(contacts);
}

void printContacts() {
	if (contacts) {
		printf("----CONTACTS----\n|\n");
		for (int i = 0; i < 27; i++) {
			if (contacts->book[i]) {
				ContactPage *cur = contacts->book[i];
				if (i < 26) {
					printf("----PAGE_%c----\n", i + 65);
				} else {
					printf("----PAGE_*----\n");
				}
				while (cur) {
					if (cur->data) {
						printContact(cur->data);
					}
					cur = cur->next;
				}
				printf("----______----\n|\n");
			}
		}
		printf("\n");
	}
}

void saveContacts() {
	FILE *fptr = fopen("contacts.sav", "w");
	if (fptr != NULL) {
		void *buffer = calloc(1, contacts->byteSize + sizeof(int));
		memcpy(buffer, &(contacts->byteSize), sizeof(int));
		void *tmp = buffer + sizeof(int);
		for (int i = 0; i < 27; i++) {
			ContactPage *cur = contacts->book[i];
			while(cur) {
				if (cur->data) {
					int len = contactLength(cur->data);
					void *b = packContact(cur->data);
					memcpy(tmp, b, len);
					free(b);
					tmp += len;
				}
				cur = cur->next;
			}
		}
		fwrite(buffer, contacts->byteSize + sizeof(int), 1, fptr);
		free(buffer);
		fclose(fptr);
	}
}

void loadContacts() {
	FILE *fptr = fopen("contacts.sav", "r");
	if (fptr != NULL) {
		int size = 0;
		fread(&size, sizeof(int), 1, fptr);
		void *buff = calloc(1, size);
		fread(buff, size, 1, fptr);
		fclose(fptr);
		void *tmp = buff;
		while (tmp - buff < size) {
			Contact *c = unpackContact(tmp);
			addToContacts(c);
			tmp += contactLength(c);
		}
		free(buff);
	}
}

/*
Blockchain *initContactBook() {
	Blockchain *conBook = makeBlockchain(printContactBlock);
	return conBook;
}

void addContact(Blockchain *bc, User *u) {
	Contact *c = getContact(u);
	void *buff = packContact(c);
	Data *d = makeData(buff, contactLength(c));
	Block *b =  makeBlock(d, bc->cur);
	addBlock(bc, b);
	freeContact(c);
}

void printContactBlock(Block *b) {
	printf("pre: ");
	printHash(b->preHash);
	printf("data: ");
	printHash(b->dataHash);
	if (b->data != NULL) {
		Contact *c = unpackContact(b->data->arr);
		printContact(c);
		free(c->name);
		free(c);
		//printDataInt(b->data);	
	}
	printf("proof: %i\n", b->proof);
	printf("-------------------\n");

}
*/
int contactLength(Contact *c) {
	return sizeof(int) + strlen(c->name) + 1 + sizeof(Key);
}

void printUser(User *u) {
	printf("User:\n%s\n", u->name);
	printf("Private Key: (n = %"PRId64", d = %"PRId64")\n", u->privateKey.n, u->privateKey.c);
	printf("Public Key: (n = %"PRId64", d = %"PRId64")\n", u->publicKey.n, u->publicKey.c);

}


void printContact(Contact *c) {
	printf("Contact:\n%s\n", c->name);
	printf("Public Key: (n = %"PRId64", c = %"PRId64")\n", c->publicKey.n, c->publicKey.c);
}

void freeContact(Contact * c) {
	free(c->name);
	free(c);
}
