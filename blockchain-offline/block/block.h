#ifndef BLOCK
#define BLOCK
#include<time.h>
#include <stdlib.h>
#include <stdio.h>
#include <memory.h>
#include <string.h>
#include <stdbool.h>
#include "sha256/sha256pub.h"
#include "../../netThread/dataThread/data.h"
#include "contacts/contactBook.h"

typedef struct Block {
	BYTE preHash[SHA256_BLOCK_SIZE];
	BYTE dataHash[SHA256_BLOCK_SIZE];
	Data *data;
	int proof;
	struct Block *next;
	time_t timestamp;
} Block;

typedef struct Blockchain {
	Block *base;
	Block *cur;
	//void (*print)(Block*);
	int len;
} Blockchain;

extern Blockchain *myChain;

#define BLOCKSIZE 68 

Block *makeBlock(Data *d, Block *pre);
Blockchain *makeBlockchain();//void (*printFunc)(Block*));
bool addBlock(Blockchain *bc, Block *b);
void hashBlockData(Block **block, void *buff);

bool validateBlock(Block *b);
bool validateChain(Blockchain *b);
void saveChain(char *file, Blockchain * bc);
Blockchain *loadChain(char *file);//, void (*printFunc)(Block*));

Data *packChain(Blockchain *bc);
Blockchain *unpackChain(void *buffer);//, void (*printfunc)(Block*));

void printChain(Blockchain *bc, void (*printFunc)(Block*));
void freeBlockchain(Blockchain *bc);

bool proofOfWork(BYTE *hash);
void *packBlockData(Block *b);
Block *unpackBlockData(void *buff);
void hashBlock(Block *b, BYTE *buff, int guess);
BYTE *getHash(Block *b, int proof);
int blockSize(Block* b);

void printHash(BYTE *hash);
void printBlock(Block *b);
void printDataInt(Data *d);
void freeData(Data *d);
Block *freeBlock(Block *b);
void setGenesis(Block* b);
Block *getGenesis();

#endif
