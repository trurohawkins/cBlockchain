data: main.c libInpThread.a
	gcc -o data -Wall -w -pedantic main.c libInpThread.a

libInpThread.a: keyboardFunc.o threads.o	data.o input.o
	ar rs libInpThread.a $^

data.o: data.c data.h
	gcc -c -g data.c

input.o: input.c input.h
	gcc -c -g input.c

keyboardFunc.o: keyboardFunc.c keyboardFunc.h
	gcc -c -g keyboardFunc.c

threads.o: threads.c threads.h
	gcc -c -g threads.c

clean:
	rm *.o

fclean:
	rm *.o
	rm *.a
	rm data
