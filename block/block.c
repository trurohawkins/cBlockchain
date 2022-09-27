#include "block.h"

Block *genesisBlock;

Blockchain *makeBlockchain() {//void (*printFunc)(Block*)) {
	Blockchain *bc = (Blockchain*)calloc(1, sizeof(Block));
	//bc->print = printFunc;
	bc->base = makeBlock(NULL, NULL);
	bc->cur = bc->base;
	return bc;
}

Block *makeBlock(Data *d, Block *pre) {
	Block *b = (Block*)calloc(1, sizeof(Block));
	BYTE *buff = calloc(SHA256_BLOCK_SIZE, sizeof(BYTE));
	b->proof = 0;
	while (true) {
		hashBlock(pre, buff, b->proof);//b->preHash);
		if (proofOfWork(buff) || pre == NULL) {
			break;
		} else {
			b->proof++;
		}
	}
	memcpy(b->preHash, buff, sizeof(BYTE) * SHA256_BLOCK_SIZE);
	free(buff);
	if (d) {
		b->timestamp = time(NULL);
	} else {
		//genesis block
		b->timestamp = 0;
	}
	b->data = d;
	hashBlockData(&b, b->dataHash);
	return b;
}

void hashBlockData(Block **block, void *buff) {
	Block *b = *block;
	if (b->data) {
		int hSize = sizeof(time_t) + (b->data ? b->data->byteSize : 0);
		void *hBuff = calloc(hSize, 1);
		memcpy(hBuff, &(b->timestamp), sizeof(time_t));
		memcpy(hBuff + sizeof(time_t), b->data->arr, b->data->byteSize);
		sha256Hash(hBuff, buff, hSize);
		free(hBuff);
	} else {
		for (int i =0; i < SHA256_BLOCK_SIZE; i++) {
			b->dataHash[i] = 0;
		}
	}
}

bool validateBlock(Block* b) {
	BYTE *hash = calloc(SHA256_BLOCK_SIZE, sizeof(BYTE));
	hashBlockData(&b, hash);
	//sha256Hash(b->data->arr, hash, b->data->byteSize);
	int res = memcmp(hash, b->dataHash, SHA256_BLOCK_SIZE);
	free(hash);
	return res == 0;
}


bool proofOfWork(BYTE *hash) {
	for (int i = 0; i < 2; i++) {
		if (hash[i] != 0) {
			return false;
		}
	}
	return true;
}

void *packBlockData(Block *b) {
	int size = blockSize(b);
	void *buff = calloc(1, size);
	size = (b->data ? b->data->byteSize : 0); 
	//size -= sizeof(int); //so when we pull out the size it doesnt include itself in the next grab
	memcpy(buff, &(size), sizeof(int));
	memcpy(buff + sizeof(int), b->preHash, 32);//sizeof(b->preHash));
	memcpy(buff + sizeof(int) + 32, b->dataHash, 32);
	memcpy(buff + sizeof(int) + 64, &(b->proof), sizeof(int));
	memcpy(buff + sizeof(int) + 64 + sizeof(int), &(b->timestamp), sizeof(time_t));
	if (b->data) {
		memcpy(buff + sizeof(int) + 64 + sizeof(int) + sizeof(time_t), b->data->arr, b->data->byteSize);
	} 
	return buff;
}

Block *unpackBlockData(void *buff) {
	Block *b = (Block*)calloc(1, sizeof(Block));
	int dataBytes;
	memcpy(&dataBytes, buff, sizeof(int));
	buff += sizeof(int); // to skip over the size
	memcpy(b->preHash, buff, 32);
	buff += 32;
	memcpy(b->dataHash, buff, 32);
	buff += 32;
	memcpy(&(b->proof), buff, sizeof(int));
	buff += sizeof(int);
	memcpy(&(b->timestamp), buff, sizeof(time_t));
	buff += sizeof(time_t);
	if (dataBytes > 0) {
		//buff += sizeof(int);
		b->data = (Data*)calloc(1, sizeof(Data));
		b->data->arr = calloc(1, dataBytes);
		memcpy(b->data->arr, buff, dataBytes);
		b->data->byteSize = dataBytes;
	} else {
		b->data = 0;
	}
	return b;
}

Data *packChain(Blockchain *bc) {
	int size = sizeof(int);
	Block *cur = bc->base->next;
	while (cur) {
		size += blockSize(cur);
		cur = cur->next;
	}
	void *buffer = calloc(size, 1);
	memcpy(buffer, &(bc->len), sizeof(int));
	void *tmp = buffer + sizeof(int);
	cur = bc->base->next;
	while (cur) {
		int bs = blockSize(cur);	
		void *buff = packBlockData(cur);
		memcpy(tmp, buff, bs);
		free(buff);
		tmp += bs;// + sizeof(int);
		cur = cur->next;
	}
	Data *d = makeData(buffer, size);
	return d;
}

Blockchain *unpackChain(void *buffer) {//, void (*printFunc)(Block*)) {
	Blockchain *bc = makeBlockchain();//printFunc);
	int len = 0;
	void *buff = buffer;
	memcpy(&len, buff, sizeof(int));
	buff += sizeof(int);
	for (int i = 0; i < len; i++) {
		Block *b = unpackBlockData(buff);
		addBlock(bc, b);
		buff += blockSize(b);// + BLOCKSIZE;// + sizeof(int);
	}	
	//expecting Data buffer, deal with outside
	///free(buffer);
	return bc;
}


void hashBlock(Block *b, BYTE *buff, int guess) {
	if (b != NULL) {
		void *data = packBlockData(b);
		int size = blockSize(b) + sizeof(int);
		void *dataProof = calloc(1, size);
		memcpy(dataProof, &guess, sizeof(int));
		memcpy(dataProof + sizeof(int), data, blockSize(b));
		sha256Hash(dataProof, buff, size);
		free(data);
		free(dataProof);
	} else {
		BYTE text[] = {"fish man"};
		sha256Hash(text, buff, strlen(text));
	}
}

bool addBlock(Blockchain *bc, Block *b) {
	Block *old = bc->cur;
	if (bc->cur) {
		bc->cur->next = b;
	} else {
		bc->base = b;
	}
	bc->cur = b;
	if (validateChain(bc)) {
		bc->len++;
		return true;
	} else {
		old->next = NULL;
		bc->cur = old;
		return false;
	}
}

BYTE *getHash(Block *b, int proof) {
	BYTE *hash = (BYTE*)calloc(SHA256_BLOCK_SIZE, sizeof(BYTE));
	hashBlock(b, hash, proof);
	return hash;
}

int blockSize(Block *b) {
	return BLOCKSIZE + (b->data ? b->data->byteSize : 0) + sizeof(int) + sizeof(time_t);
}

bool validateChain(Blockchain *bc) {
	Block *cur = bc->base;
	while (cur != NULL) {
		Block *pre = cur;
		cur = cur->next;
		if (cur != NULL) {
			BYTE *hash = getHash(pre, cur->proof);
			if (validateBlock(cur) && memcmp(hash, cur->preHash, SHA256_BLOCK_SIZE) != 0) {
				free(hash);
				return false;
			}
			free(hash);
		}
	}
	return true;
}

void saveChain(char *file, Blockchain *bc) {
	FILE *fptr = fopen(file, "wb");
	if (fptr != NULL) {
		Data *d = packChain(bc);
		//size_t len = d->byteSize + sizeof(int);
		fwrite(&(d->byteSize), sizeof(int), 1, fptr);
		fwrite(d->arr, d->byteSize, 1, fptr);
		freeData(d);
		fclose(fptr);
	}
}

Blockchain *loadChain(char *file) {//, void (*printFunc)(Block*)) {
	FILE *fptr = fopen(file, "rb");
	Blockchain *chain = 0;//makeBlockchain();//printFunc);//(Blockchain*)calloc(1, sizeof(Blockchain));
	if (fptr != NULL) {
		int size;
		fread(&size, sizeof(int), 1, fptr);
		void *buff = calloc(1, size);
		fread(buff, size, 1, fptr);
		chain = unpackChain(buff);	
		free(buff);
		fclose(fptr);
	}
	return chain;
}

void printHash(BYTE *hash) {
	for (int i = 0; i < SHA256_BLOCK_SIZE; i++) {
		printf("%x", hash[i]);
	}
	printf("\n");
}

void printBlock(Block *b) {
	//printf("-------------------\n");
	printf("prev: ");
	printHash(b->preHash);
	printf("data: ");
	printHash(b->dataHash);
	if (b->data != NULL) {
		printDataInt(b->data);	
	}
	printf("proof: %i\n", b->proof);
	printf("-------------------\n");
}

void printDataInt(Data *d) {
	printf("data:\n%i\n", *((int*)(d->arr)));
}

void printChain(Blockchain *bc, void (*printBlock)(Block*)) {
	int count = 0;
	Block *cur = bc->base;
	while (cur != NULL) {
		printf("Block #%i\n", count);
		printBlock(cur);
		//printBlock(cur, bc->print);
		count++;
		cur = cur->next;
	}
}

Data *makeData(void *arr, int bytes) {
	Data *d = (Data*)calloc(1, sizeof(Data));
	d->arr = arr;
	d->byteSize = bytes;
	return d;
}

void freeData(Data *d) {
	free(d->arr);
	free(d);
}

Block *freeBlock(Block *b) {
	if (b->data) {
		freeData(b->data);
	}
	Block *next = b->next;
	free(b);
	return next;
}

void freeBlockchain(Blockchain *bc) {
	Block *c = bc->base;
	while (c != NULL) {
		c = freeBlock(c);
	}
	free(bc);
}

void setGenesis(Block* b) {
	genesisBlock = b;
}

Block *getGenesis() {
	return genesisBlock;
}

