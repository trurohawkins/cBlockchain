#include <time.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "prime.h"
#include "../../../../netThread/dataThread/data.h"

typedef struct Key {
	NUM n;
	NUM c;
} Key;

NUM cryptItem(NUM data, NUM c, NUM n);
NUM *cryptData(NUM *data, int len, Key user);
Key *makeKeyPair();
void printKey(Key *k);
bool compareKeys(Key k1, Key k2);
void *packKey(void *buff, Key *k);
Key *unpackKey(void *buff);

char cryptByte(char data, Key user);
