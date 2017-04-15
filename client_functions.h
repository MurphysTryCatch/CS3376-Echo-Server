#ifndef CLIENT_FUNCTIONS_H
#define CLIENT_FUNCTIONS_H

void error(const char *msg);
int sendMessage(char buffer[], int sockfd);
int readMessage(char buffer[], int sockfd);

#endif