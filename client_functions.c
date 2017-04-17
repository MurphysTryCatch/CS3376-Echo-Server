// Client_functions.c
// Written by Linda Murphy
// 
// Common functions called by client.c

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

#include "client_functions.h"

void error(const char *msg) {
    perror(msg);
    exit(1);
}

int sendMessage(char buffer[], int sockfd) {
	printf("Please enter the message: ");
    bzero(buffer, 256);
    fgets(buffer, 255, stdin);
    int n = write(sockfd, buffer, strlen(buffer));
    printf("Here is the message: %s\n", buffer);
    return n;
}

int readMessage(char buffer[], int sockfd) {
	bzero(buffer, 256);
    return read(sockfd, buffer, 255);

}