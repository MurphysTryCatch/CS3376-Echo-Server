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
void serverReadWrite(int sock) {
	char buffer[256];
	bzero(buffer, 256);

	if (read(sock, buffer, 255) < 0) {
        error("ERROR reading from socket");
    }

    if (write(sock, "I got your message", 18) < 0) {
        error("ERROR writing to socket");
    }
}