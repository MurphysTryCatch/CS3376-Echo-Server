#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h> 
#include "server_functions.h"

int readSocket(char buffer[], int newsockfd) {
	bzero(buffer,256);
    int n = read(newsockfd,buffer,255);
    return n;
}

int writeSocket(int newsockfd) {
	int n = write(newsockfd,"I got your message",18);
	return n;
}