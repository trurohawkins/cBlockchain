COM = gcc -Wall -w -pedantic -c -g

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

libNetThread.a:  networkCommand.o $(P)node.o $(D)input.o $(D)threads.o	$(N)client.o $(N)server.o
	ar rs libNetThread.a $^

networkCommand.o: networkCommand.c networkCommand.h
	$(COM) networkCommand.c

node.o: $(P)node.c $(P)node.h $(P)net.c
	$(MAKE) node.o $(P)

$(D)input.o: $(D)data.c $(D)data.h $(D)input.c $(D)input.h
	$(MAKE) input.o -C $(D)

threads.o: $(D)threads.c $(D)threads.h
	$(MAKE) threads.c $(D)

client.o: $(N)client.c $(N)client.h
	$(COM) $(N)client.c

server.o: $(N)server.c $(N)server.h
	$(COM) $(N)server.c

libBlock.a: $(BC)transactionPool.o sha256.o $(B)block.o $(C)contactBook.o $(T)transaction.o $(T)rsa.o
	ar rs libBlock.a $^

transactionPool.o: $(BC)transactionPool.c $(BC)transactionPool.h
	$(COM) $(BC)transactionPool.c

block.o: $(B)block.c $(B)block.h
	$(COM) $(B)block.c

contactBook.o:$(C)contactBook.c$(C)contactBook.h
	$(COM)$(C)contactBook.c

$(T)transaction.o: $(T)transaction.c $(T)transaction.h
	$(MAKE) transaction.o -C $(T)

$(T)rsa.o:
	$(MAKE) rsa.o -C $(T)

sha256.o: $(S)sha256.c $(S)sha256.h
	$(COM) $(S)sha256.c 

test:
	valgrind --fair-sched=yes --leak-check=full --show-leak-kinds=all ./blockchain

test2:
	valgrind --fair-sched=yes --leak-check=full --show-leak-kinds=all ./blockchain 127.0.0.1

clean:
	rm *.o
	rm vgcore*

fclean:
	rm *.o
	$(MAKE) clean -C $(BC)
	$(MAKE) clean -C $(B)
	$(MAKE) clean -C $(N)
	$(MAKE) clean -C $(T)
	$(MAKE) clean -C $(P)
	$(MAKE) clean -C $(C)
	$(MAKE) clean -C $(D)
	rm *.a
	rm blockchain
	rm vg*
	rm *.u
	rm *.sav



