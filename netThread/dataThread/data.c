bool littleEndian() {
	unsigned int i = 1;
	char *c = (char*)&i;
	if (*c) {
		return true;
	} else {
		return false;
	}
}

Data *makeData(void *arr, int bytes) {
	Data *d = (Data*)calloc(1, sizeof(Data));
	d->arr = calloc(1, bytes);
	memcpy(d->arr, arr, bytes);
	d->bytes = bytes;
	return d;
}

void *writeData(Data *d) {
	void *buff = calloc(1, d->bytes + sizeof(int));
	int size = htonl(d->bytes);
	memcpy(buff, &size, sizeof(int));
	void *bu = d->arr;;
	if (littleEndian()) {
		bu = flipEndian(d->arr, d->bytes);
	}
	//memcpy(buff, &d->bytes, sizeof(int));
	memcpy(buff + sizeof(int), bu, d->bytes);
	if (littleEndian()) {
		free(bu);
	}
	return buff;
}

void *flipEndian(void *buff, int size) {
	void *bu = calloc(size, 1);
	memcpy(bu, buff, size);
	int s = size; 
	char *ptr = (char *)bu;
	for (int i = 0; i < s / 2; i++) {
		char tmp = ptr[s-1-i];
		ptr[s-1-i] = ptr[i];
		ptr[i] = tmp;
	}
	return bu;
}

Data *readData(void *buffer) {
	int size = *(int*)buffer;
	if (littleEndian()) {
		size = htonl(size);
	}
	if (size < BUFF - sizeof(int) && size > 0) {
		Data *d = (Data*)calloc(1, sizeof(Data));
		d->bytes = size;
		d->arr = calloc(1, d->bytes);
		memcpy(d->arr, buffer + sizeof(int), d->bytes);
		if (littleEndian()) {
			void *bu = flipEndian(d->arr, size);
			free(d->arr);
			d->arr = bu;
		}
		return d;
	} else {
		printf("bad read byte size: %i,should be > 0 &&  < %i\n", size, BUFF - sizeof(int));
		return 0;
	}
}

void freeData(Data *d) {
	free(d->arr);
	free(d);
}

void printBinary(void *buff, int bytes) {
	int tmp;
	bytes *= 8;
	int n = *(int*)buff;
	int *binary = (int*)calloc(bytes, sizeof(int));
	for (int i = 0; i < bytes; i++) {
		//tmp = (*(int*)buff >> (bytes - i - 1));
		if (n&1) {
			binary[bytes-i] = 1;
		} else {
			binary[bytes-i] = 0;
		}
		n >>= 1;
	}
	for (int i = 1; i <= bytes; i++) {
		printf("%d", binary[i]);
		if ((i-1) % 8 == 7 ) {
			printf(" ");
		}

	}
	printf("\n");
}

