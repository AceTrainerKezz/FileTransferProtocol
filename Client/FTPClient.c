#include <stdio.h>  
#include <sys/types.h> 
#include <unistd.h> 
#include <stdlib.h>  
#include <errno.h>   
#include <sys/wait.h>
#include <string.h>
#include "CMDHandler.h"
int main () 
{
	int SIZE = 10000;   
	char str[SIZE];    
	char cwd[SIZE];
	const char s[2] = " \n";
	char *command, *arg;
	
	while(1)
	{
		printf(" $>");
		fgets(str,sizeof(str), stdin);
	
		command = strtok(str, s);
		
		arg = strtok(NULL, s);

		CmdHandler(command, arg);
	}
	return(0);
}

