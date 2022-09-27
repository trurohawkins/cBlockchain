both: client server

client: client.c client.h clientMain.c
	gcc -o client -g clientMain.c

server: server.c server.h main.c
	gcc -o server -g main.c

clean:
	rm *.o

fclean:
	rm client
	rm server
	rm vg*
