/*
 * file: daemon.h
 * author: Kieron van der Kwast
 * aim: create daemon process
 * date: 13/10/2018
 */

//initialise daemon process
void DaemonInit();

//claim as many zombies as we can
void ClaimChildren();

//handle a client program
void ServeAClient(int sd);

