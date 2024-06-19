FTPClient: FTPClient.o CMDHandler.o
	gcc FTPClient.o CMDHandler.o -o FTPClient

FTPClient.o: FTPClient.c CMDHandler.h
	gcc -c FTPClient.c

CMDHandler.o: CMDHandler.c CMDHandler.h
	gcc -c CMDHandler.c
clean: 
	rem*.o
