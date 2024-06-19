/*
 * file: daemon.c
 * author: Kieron van der Kwast
 * aim: implement daemon process
 * date: 13/10/2018
 */

#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include "daemon.h"

#define MAX_BLOCK_SIZE (1024*5)	//max size of any piece of data that can be sent by client
				// should be in the protocol's .h

void DaemonInit()
{
	pid_t pid;
	struct sigaction act;

	if( (pid = fork() ) < 0) 
	{
		perror("fork");
		exit(1);
	}
	else if(pid > 0)
	{
		printf("server pid = %d\n", pid);
		exit(0);	//parent terminates
	}

	//child process continues
	setsid();	//become session leader
	chdir("/");	//change working directory
	umask(0); 	//clear file mode creation mask

	//catch SIGCHILD to remove zombies from system
	act.sa_handler = ClaimChildren;		//use reliable signal
	sigemptyset(&act.sa_mask);		//not to block other signals
	act.sa_flags = SA_NOCLDSTOP;		//not catch stopped children
	sigaction(SIGCHLD, (struct sigaction *)&act, (struct sigaction *)0);
}

void ClaimChildren()
{
	pid_t pid = 1;

	while(pid > 1)
	{	//claim as many zombies as we can
		pid = waitpid(0, (int *)0, WNOHANG);
	}
}

void ServeAClient(int sd)
{
	int nr, nw;
	char buf[MAX_BLOCK_SIZE];
	
	while (1)
	{
		//read data from client
		//if((nr = readn(sd, buf, sizeof(buf))) <= 0)
		//{
			//return;	//connection broken down
		//}
		//process data
		
		
		//send results to client
		
	}	
}







