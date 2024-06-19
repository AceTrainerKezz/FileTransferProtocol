/* file: server.c
 * aim: implement daemon server
 * author: Kieron van der Kwast
 * date: 13/10/2018
 */

#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <signal.h>
#include <syslog.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <netinet/in.h>
#include <unistd.h>
#include "daemon.h"

#define logfile "./server.log"
#define SERV_TCP_PORT 40005	//default server listening port


int main(int argc, char *argv[])
{
	FILE *log;
	int sd, nsd, n;
	pid_t pid;
	unsigned short port;	//server listening port
	socklen_t cliAddrLen;
	struct sockaddr_in serAddr, cliAddr;

	//create a log file
	log = fopen(logfile, "w+");
	if(!log)
	{
		fprintf(stderr, 
			"Error: could not create log file %s\n", logfile);
		exit(1);
	}
	
	//get port number
	if(argc == 1)
	{
		port = SERV_TCP_PORT;
	}
	else if (argc == 2)
	{
		int n = atoi(argv[1]);
		if(n >= 1024 && n < 65536)
		{
			port = n;
		}
		else 
		{
			fprintf(log,
				"Error: port number must be between 1024 and 65535\n");
			exit(1);
		}
	}
	else
	{
		fprintf(log, "Usage: %s [ server listening port ]\n", argv[0]);
		exit(1);
	}

	//turn process into a daemon
	DaemonInit();

	//log daemon pid
	pid = getpid();
	fprintf(log, "My pid is %d\n", pid);
	fflush(log);
	
	//set up listening socket sd
	if((sd = socket(PF_INET, SOCK_STREAM, 0)) < 0)
	{
		perror("server:socket");
		exit(1);
	}
	
	//build server Internet socket address
	bzero((char *)&serAddr, sizeof(serAddr));
	serAddr.sin_family = AF_INET;
	serAddr.sin_port = htons(port);
	serAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	//accept client request sent to any one of the network
	//interface(s) on this host.

	//bind server address to socket sd
	if(bind(sd, (struct sockaddr *) &serAddr, sizeof(serAddr)) < 0)
	{
		perror("server bind");
		exit(1);
	}
	
	//become a listening socket
	listen(sd, 5);
	
	//do stuff
	while(1)
	{
		//wait to accept a client request for connection
		cliAddrLen = sizeof(cliAddr);
		nsd = accept(sd, (struct sockAddr *) &cliAddr, &cliAddrLen);
		if(nsd < 0)
		{
			if(errno == EINTR)	//if interrupted by SIGCHILD
			{
				continue;
			}
			perror("Server:accept"); 
			exit(1);
		}
		
		//create a child process to handle this client
		if((pid = fork()) < 0)
		{
			perror("fork");
			exit(1);
		}
		else if(pid > 0)
		{
			close(nsd);
			continue;	//parent to wait for next client
		}

		//now in child, serve the current client
		close(sd);
		ServeAClient(nsd);
		exit(0);
	}
}





