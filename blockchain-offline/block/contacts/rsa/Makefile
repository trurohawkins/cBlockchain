COM = gcc -c -g -Wall -w -pedantic

transaction: main.o libTransaction.a
	gcc -o transaction main.o libTransaction.a -lm    

main.o: main.c
	$(COM) main.c

libTransaction.a: transaction.o rsa.o
	ar rs libTransaction.a transaction.o rsa.o

transaction.o: transaction.c transaction.h
	$(COM) transaction.c

rsa.o: RSA.c RSA.h rsaCrypt.c rsaCrypt.h prime.c prime.h
	gcc -o rsa.o -c -g rsaCrypt.c
 
clean:
	rm *.o

fclean:
	rm *.o
	rm *.a
	rm transaction
	rm vg*
