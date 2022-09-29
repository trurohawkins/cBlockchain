S = sha256/
C = contacts/
T = $(C)rsa/
D = ../../netThread/dataThread/

block: main.c libBlock.a
	gcc -o block -g -Wall -w -pedantic main.c libBlock.a -lm

libBlock.a: sha256.o block.o $(C)contactBook.o $(T)transaction.o $(T)rsa.o $(D)data.o
	ar rs libBlock.a $^

block.o: block.c block.h
	gcc -c -g block.c

contactBook.o:$(C)contactBook.c$(C)contactBook.h
	gcc -c -g  $(C)contactBook.c

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
	$(MAKE) clean -C $(T)
	$(MAKE) clean -C $(C)
	$(MAKE) clean -C $(D)
	rm *.a
	rm block
	rm *.u
	rm *.sav
	rm vg*

