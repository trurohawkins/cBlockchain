B = block/
S = $(B)sha256/
C = $(B)contacts/
T = $(C)rsa/
D = ../netThread/dataThread/

blockchain: main.c libBlock.a
	gcc -o blockchain -g -Wall -w -pedantic main.c libBlock.a -lm

libBlock.a: transactionPool.o sha256.o $(B)block.o $(C)contactBook.o $(T)transaction.o $(T)rsa.o $(D)data.o
	ar rs libBlock.a $^

transactionPool.o: transactionPool.c transactionPool.h
	gcc -c -g transactionPool.c

block.o: $(B)block.c $(B)block.h
	gcc -c -g $(B)block.c

contactBook.o:$(C)contactBook.c$(C)contactBook.h
	gcc -c -g$(C)contactBook.c

$(D)data.o: $(D)data.c $(D)data.h
	$(MAKE) data.o -C $(D)

$(T)transaction.o: $(T)transaction.c $(T)transaction.h
	$(MAKE) transaction.o -C $(T)

$(T)rsa.o:
	$(MAKE) rsa.o -C $(T)

sha256.o: $(S)sha256.c $(S)sha256.h
	gcc -c -g $(S)sha256.c 

clean:
	rm *.o

fclean:
	rm *.o
	$(MAKE) clean -C $(B)
	$(MAKE) clean -C $(T)
	$(MAKE) clean -C $(C)
	rm *.a
	rm blockchain
	rm *.u
	rm *.sav
	rm vg*


