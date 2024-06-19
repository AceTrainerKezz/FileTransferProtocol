# makefile for server

daemonserver: server.o daemon.o
	gcc server.o daemon.o -o daemonserver

server.o: server.c daemon.h
	gcc -c server.c

daemon.o: daemon.c daemon.h
	gcc -c daemon.c

clean:
	rm *.o
