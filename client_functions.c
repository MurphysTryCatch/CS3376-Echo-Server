#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h> 
#include "client_functions.h"

int sendMessage(char buffer[], int sockfd) {
	printf("Please enter the message: ");
    bzero(buffer,256);
    fgets(buffer,255,stdin);
    int n = write(sockfd,buffer,strlen(buffer));
    printf("Here is the message: %s\n",buffer);
    return n;
}

int readMessage(char buffer[], int sockfd) {
	bzero(buffer,256);
    int n = read(sockfd,buffer,255);
    return n;
}