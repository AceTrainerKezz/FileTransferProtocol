#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "CMDHandler.h"
#include <sys/types.h> 
#include <unistd.h> 
#include <stdlib.h>  
#include <errno.h>   
#include <sys/wait.h>

int CmdHandler(const char* cmd, const char* arg)
{
	
	if(strcmp(cmd, CliDir) == 0)
	{
		ClientCMDnoArg(DIRCMD);
	}
	else if(strcmp(cmd, CliPwd) == 0)
	{
		ClientCMDnoArg(PWDCMD);
	}
	else if(strcmp(cmd, CliCd) == 0)
	{
		CliendtCMDwArg( cmd, arg);
	}
	else if(strcmp(cmd, QUIT) == 0)
	{
		exit(0);
	}
	else
	{
		printf("ERROR");
	}

	return 0;
}
void CliendtCMDwArg(const char* cmd, const char* arg)
{
	chdir(arg);
}
const char* GetCWD()
{
	int SIZE = 10000;   
	char cwd[SIZE];
	getcwd(cwd, sizeof(cwd));
}
int ClientCMDnoArg(const char* cmd)
{
	pid_t pid;
	int status = 0;

	pid = fork();
	if(pid == 0)//child process
	{
		execlp(cmd,cmd,NULL);//runs command
		exit(10);
	}
	else if(pid > 0)//parent process waits till child finishes
	{
		wait(&status);
		printf("child has exited\n");
	}
	else//error when child cant be created
	{
		perror("fork");
		exit(1);
	}
	return 1;
}
