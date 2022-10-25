COM = gcc -Wall -w -pedantic -c -g

data: main.c libInpThread.a
	gcc -o data -g -Wall -w -pedantic main.c libInpThread.a

libInpThread.a: input.o threads.o
	ar rs libInpThread.a $^

input.o: input.c input.h data.c data.h
	$(COM) input.c

threads.o: threads.c threads.h
	$(COM) threads.c

test:
	valgrind --fair-sched=yes --leak-check=full --show-leak-kinds=all ./data

clean:
	rm *.o

fclean:
	rm *.o
	rm *.a
	rm data
