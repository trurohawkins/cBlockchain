block: block.c block.h main.c sha256/sha256.h sha256/sha256.c libsha256.a
	gcc -o block -g -Wall -w -pedantic main.c libsha256.a

libsha256.a: sha256.o
	ar rs libsha256.a sha256.o

sha256.o: sha256/sha256.c sha256/sha256.h
	gcc -c -g sha256/sha256.c 

clean:
	rm block
	rm *.o
	rm vgcore*
