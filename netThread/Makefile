N = network/
D = dataThread/

node: main.c libNetThread.a
	gcc -o node -g -Wall -w -pedantic main.c libNetThread.a

libNetThread.a:  node.o $(D)dataInput.o $(D)keyboardFunc.o $(D)threads.o	$(N)client.o $(N)server.o
	ar rs libNetThread.a $^

node.o: node.c node.h
	gcc -c -g node.c

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

clean: 
	rm *.o

fclean:
	rm *.o
	rm *.a
	$(MAKE) clean -C $(D)
	$(MAKE) clean -C $(N)
	rm node
	rm vg*

