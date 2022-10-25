COM = gcc -c -g -Wall -w -pedantic
T = rsa/

contactBook: main.o libContacts.a
	gcc -o contactBook main.o libContacts.a -lm

main.o: main.c
	$(COM) main.c

libContacts.a: contactBook.o $(T)transaction.o $(T)rsa.o  
	ar rs libContacts.a contactBook.o $(T)transaction.o $(T)rsa.o 

contactBook.o: contactBook.c contactBook.h
	$(COM) contactBook.c

$(T)transaction.o: $(T)transaction.c $(T)transaction.h
	$(MAKE) transaction.o -C $(T)

$(T)rsa.o:
	$(MAKE) rsa.o -C $(T)

clean:
	rm *.o

fclean:
	rm *.o
	$(MAKE) clean -C $(T)
	rm *.a
	rm contactBook
	rm *.u
	rm *.sav
	rm vg*
