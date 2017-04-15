#ifndef CLIENT_FUNCTIONS_H
#define CLIENT_FUNCTIONS_H

int sendMessage(char buffer[], int sockfd);
int readMessage(char buffer[], int sockfd);

#endif