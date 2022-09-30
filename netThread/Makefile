N = network/
D = dataThread/

node: main.c libNetThread.a
	gcc -o node -g -Wall -w -pedantic main.c libNetThread.a

libNetThread.a:  node.o $(D)data.o $(D)input.o $(D)keyboardFunc.o $(D)threads.o	$(N)client.o $(N)server.o
	ar rs libNetThread.a $^

node.o: node.c node.h
	gcc -c -g node.c

data.o: $(D)data.c $(D)data.h
	gcc -c -g $(D)data.c

input.o: $(D)input.c $(D)input.h
	gcc -c -g $(D)input.c

keyboardFunc.o: $(D)keyboardFunc.c $(D)keyboardFunc.h
	gcc -c -g $(D)keyboardFunc.c

threads.o: $(D)threads.c $(D)threads.h
	gcc -c -g $(D)threads.c

client.o: $(N)client.c $(N)client.h
	gcc -c -g $(N)client.c

server.o: $(N)server.c $(N)server.h
	gcc -c -g $(N)server.c

test:
	valgrind --fair-sched=yes --leak-check=full --show-leak-kinds=all ./node

clean: 
	rm *.o

fclean:
	rm *.o
	rm *.a
	$(MAKE) clean -C $(D)
	$(MAKE) clean -C $(N)
	rm node
	rm vg*

