#ifndef SERVER_FUNCTIONS_H
#define SERVER_FUNCTIONS_H

void error(const char *msg);
int readSocket(char buffer[], int newsockfd); 
int writeSocket(int newsockfd);

#endif