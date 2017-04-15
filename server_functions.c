#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h> 

#include "server_functions.h"

void error(const char *msg) {
    perror(msg);
    exit(1);
}

int readSocket(char buffer[], int newsockfd) {
	bzero(buffer, 256);
    return read(newsockfd, buffer, 255);
}

int writeSocket(int newsockfd) {
	return write(newsockfd, "I got your message", 18);
}