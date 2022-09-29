BC = blockchain-offline/
B = $(BC)block/
S = $(B)sha256/
C = $(B)contacts/
T = $(C)rsa/

P = netThread/
N = $(P)network/
D = $(P)dataThread/


blockchain: main.c libBlock.a libNetThread.a
	gcc -o blockchain -g -Wall -w -pedantic main.c libNetThread.a libBlock.a -lm

libNetThread.a:  networkCommand.o $(P)node.o $(D)dataInput.o $(D)keyboardFunc.o $(D)threads.o	$(N)client.o $(N)server.o
	ar rs libNetThread.a $^

networkCommand.o: networkCommand.c networkCommand.h
	gcc -c -g networkCommand.c

node.o: $(P)node.c $(P)node.h
	gcc -c -g $(P)node.c

dataInput.o: $(D)dataInput.c $(D)dataInput.h
	gcc -c -g $(D)dataInput.c

keyboardFunc.o: $(D)keyboardFunc.c $(D)keyboardFunc.h
	gcc -c -g $(D)keyboardFunc.c

threads.o: $(D)threads.c $(D)threads.h
	gcc -c -g $(D)threads.c

client.o: $(N)client.c $(N)client.h
	gcc -c -g $(N)client.c

server.o: $(N)server.c $(N)server.h
	gcc -c -g $(N)server.c

libBlock.a: $(BC)transactionPool.o $(S)sha256.o $(B)block.o $(C)contactBook.o $(T)transaction.o $(T)rsa.o
	ar rs libBlock.a $^

transactionPool.o: $(BC)transactionPool.c $(BC)transactionPool.h
	gcc -c -g $(BC)transactionPool.c

block.o: $(B)block.c $(B)block.h
	gcc -c -g $(B)block.c

contactBook.o:$(C)contactBook.c$(C)contactBook.h
	gcc -c -g$(C)contactBook.c

$(T)transaction.o: $(T)transaction.c $(T)transaction.h
	$(MAKE) transaction.o -C $(T)

$(T)rsa.o:
	$(MAKE) rsa.o -C $(T)

sha256.o: $(S)sha256.c $(S)sha256.h
	gcc -c -g $(S)sha256.c 

clean:
	rm *.o
	rm vgcore*

fclean:
	rm *.o
	$(MAKE) clean -C $(B)
	$(MAKE) clean -C $(T)
	$(MAKE) clean -C $(P)
	$(MAKE) clean -C $(C)
	$(MAKE) clean -C $(D)
	rm *.a
	rm blockchain
	rm *.u
	rm *.sav
	rm vg*



