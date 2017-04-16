#ifndef SERVER_FUNCTIONS_H
#define SERVER_FUNCTIONS_H

void error(const char *msg);
void serverReadWrite(int sock);
void serverReadWriteUdp(int sock); 

#endif
