// Written by Matthew Dumler and Linda Murphy
#ifndef SERVER_FUNCTIONS_H
#define SERVER_FUNCTIONS_H

void error(const char *msg);
void serverReadWrite(int sock,char *ip, int port);
void serverReadWriteUdp(int sock, char *ip, int port); 

#endif
