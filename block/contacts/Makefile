T = rsa/

contactBook: main.o libContacts.a
	gcc -o contactBook main.o libContacts.a -lm

main.o: main.c
	gcc -c -g main.c

libContacts.a: contactBook.o $(T)transaction.o $(T)rsa.o  
	ar rs libContacts.a contactBook.o $(T)transaction.o $(T)rsa.o 

contactBook.o: contactBook.c contactBook.h
	gcc -c -g contactBook.c

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
	rm *.u
	rm *.sav
	rm contactBook
	rm vg*
