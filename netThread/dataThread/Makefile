data: main.c dataInput.c dataInput.h libInpThread.a
	gcc -o data -Wall -w -pedantic main.c libInpThread.a

libInpThread.a: keyboardFunc.o threads.o	
	ar rs libInpThread.a $^

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
