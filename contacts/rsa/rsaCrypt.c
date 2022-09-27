#include "prime.c"
#include "RSA.c"
#include "rsaCrypt.h"

NUM cryptItem(NUM data, NUM c, NUM n) {
	return power(data, c, n);//modular_exponentiation(data, c, n);
}

NUM *cryptData(NUM *data, int len, Key user) {
	if (user.c == 0 || user.n == 0) {
		return 0;
	}
	NUM *result = (NUM*)calloc(len, sizeof(NUM));
	for (int i = 0; i < len; i++) {
		result[i] = power(data[i], user.c, user.n);
		//result[i] = modular_exponentiation(data[i], user.c, user.n);
	}
	return result;
}

Key *makeKeyPair() {
	NUM p = randomPrime();
	NUM q = randomPrime();
	if (p == 0 || q == 0) {
		return 0;
	}
	while (p == q) {
		q = randomPrime();
	}
	NUM *k = genKeys(p, q);
	Key *keys = (Key*)calloc(2, sizeof(Key));
	//1st is the public key
	keys[0].n = k[0];
	keys[0].c = k[1];
	//2nd is the private key
	keys[1].n = k[2];
	keys[1].c = k[3];
	free(k);
	return keys;
}

void *packKey(void *buff, Key *k) {
	//void *buff = calloc(2, sizeof(k->n));
	memcpy(buff, &(k->n), sizeof(NUM));
	buff += sizeof(NUM);
	memcpy(buff, &(k->c), sizeof(NUM));
	return buff + sizeof(NUM);
}

Key *unpackKey(void *buff) {
	Key *k = (Key*)calloc(1, sizeof(Key));
	memcpy(&(k->n), buff, sizeof(NUM));
	memcpy(&(k->c), buff + sizeof(NUM), sizeof(NUM));
	return k;
}

void printKey(Key *k) {
	printf("Key: (n = %"PRId64", c = %"PRId64")\n", k->n, k->c);
}

bool compareKeys(Key k1, Key k2) {
	return k1.n == k2.n && k1.c == k2.c;
}

char cryptByte(char data, Key user) {
	return power(data, user.c, user.n);
}

