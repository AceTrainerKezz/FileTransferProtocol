

#include <sys/types.h>
#include <netinet/in.h> 
#include <unistd.h>
#include <stdio.h>  
#include <stdlib.h>  
#include <string.h>
#include <dirent.h>
#include "Protocol.h"
int sendOpcode(int fd, char opcode)
{
	write(fd, &opcode, 1);

	return 1;
}

int receiveOpcode(int fd, char *opcode)
{
	char code;
	int nr;
	nr = read(fd, &code, 1);
	*opcode = code;
	return nr;
}

int sendDirPac(int fd, char opcode, char *buff, int nr)
{

	sendOpcode(fd, opcode);
	write(fd, buff, nr);
	
	return 1;
}

int recDirPac(int fd, char *dirCont)
{
	char rec;
	char buff[MAX_BLOCK_SIZE];
	
	receiveOpcode(fd, &rec);

	if(rec == DIR_OPCODE || rec == PWD_OPCODE)
	{
		read(fd, dirCont, MAX_BLOCK_SIZE);
		memset(buff, 0, sizeof(buff));
		return 0;
	}
	else
	{
		memset(buff, 0, sizeof(buff));
		return -1;
	}
	
}
int sendReqPac(int fd, char opcode, char* arg)
{
	char buff[MAX_BLOCK_SIZE];
	int nr;
	strcpy(buff, arg);
	nr = strlen(buff);
	buff[nr] = '\0';
	if(opcode == CD_OPCODE || opcode == PUT_OPCODE || opcode == GET_OPCODE)
	{
		sendOpcode(fd, opcode);
		write(fd, buff, nr);
	}
	memset(buff, 0, sizeof(buff));	
	return 1;
}
int recReqPac(int fd, char opcode, char* reqMSG)
{	
	if(opcode == CD_OPCODE || opcode == PUT_OPCODE || opcode == GET_OPCODE)
	{
		read(fd, reqMSG, MAX_BLOCK_SIZE);		
	}
	return 1;
}
int sendAckPac(int fd, char opcode, char MSG)
{
	if(opcode == CD_OPCODE || opcode == PUT_OPCODE || opcode == GET_OPCODE)
	{
		sendOpcode(fd, opcode);
		write(fd, &MSG, 1);
	}

	return 1;
}
int recAckPac(int fd, char *buff)
{	
	char rec;
	char msg;
	receiveOpcode(fd, &rec);
	if(rec == CD_OPCODE || rec == PUT_OPCODE || rec == GET_OPCODE)
	{
		read(fd, &msg, sizeof(msg));
		*buff = msg;
		return 0;
	}
	else
		return -1;

	
}

/*void sendDataPac(int fd, FILE *fp, char opcode, FILE *log)
{	
	int nw = 0;
	int remain;
	int offset = 0;
	int numWBlocks = 0;
	int filesize;
	char buff[MAX_BLOCK_SIZE];
	

	fseek(fp, 0, SEEK_END);
	filesize = ftell(fp);
	sendDataFileSize(fd, filesize, opcode);
	remain = filesize;
	fprintf(log, "file size: %d\n", remain);
	fflush(log);

	while(remain > 0)
	{
		if(remain >= MAX_BLOCK_SIZE)
		{
			fseek(fp, offset, SEEK_SET);
			fread(buff, MAX_BLOCK_SIZE, 1, fp);
			nw =  write(fd, buff, MAX_BLOCK_SIZE);
			remain = remain - nw;
			offset = offset + nw;
			numWBlocks++;
		}
		else if(remain < MAX_BLOCK_SIZE)
			break;
	}


	char remBuff[remain];
	fseek(fp, MAX_BLOCK_SIZE * numWBlocks, SEEK_SET);
	fread(remBuff, remain, 1, fp);
	write(fd, remBuff, remain);
	memset(remBuff, 0, sizeof(remBuff));
	memset(buff, 0, sizeof(buff));
	
}
void recDataPac(int fd, FILE *fp)
{
	
	char buff[MAX_BLOCK_SIZE];
	int filesize;
	int nr = 0;
	int remain = 0;
	

	recDataFileSize(fd, &filesize);
	remain = filesize;
	printf("received data file size %d \n", remain);

	while(remain > 0)
	{
		if(remain >= MAX_BLOCK_SIZE)
		{
			nr = read(fd, buff, MAX_BLOCK_SIZE);
			nr = MAX_BLOCK_SIZE;
			fwrite("buff", nr, 1, fp);
			
			remain = remain - nr;
		}
		else if(remain < MAX_BLOCK_SIZE)
			break;		

	}
	char remBuff[remain];
	nr = read(fd, remBuff, remain);
	fwrite("remBuff", nr, 1, fp);
	memset(remBuff, 0, sizeof(remBuff));
	memset(buff, 0, sizeof(buff));
	
}*/

int sendDataPac(int fd, char *buff, int buffsize)
{
	return write(fd, buff, buffsize);
}
int recDataPac(int fd, char *buff, int buffsize)
{
	return read(fd,buff, buffsize);
}
int sendDataFileSize(int fd,  int filesize, char opcode)
{
	int tmp = filesize;
	tmp = htonl((uint32_t)tmp);
	sendOpcode(fd, opcode);
	write(fd, &tmp, sizeof(tmp));
}
int recDataFileSize(int fd, int *filesize)
{
	int tmp;
	char rec;
	receiveOpcode(fd, &rec);
	if(rec == PUT_OPCODE || rec == GET_OPCODE)
	{
		read(fd, &tmp, sizeof(tmp));
		tmp = ntohl(tmp);
		*filesize = tmp;
		return 0;
	}
	else
		return -1;
}
