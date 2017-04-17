// Server_functions.c
// Written by Linda Murphy
// 
// Common functions called by server.c

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

    //print the message to the server console
    printf("TCP message received: %s", buffer);

    if (write(sock, "I got your message", 18) < 0) {
        error("ERROR writing to socket");
    }
}

void serverReadWriteUdp(int sock) {
    char buffer [256];
    bzero(buffer, 256);

    struct sockaddr_in cli_addr;
    socklen_t clilen = sizeof(cli_addr);

    if (recvfrom(sock , buffer, 256, 0, (struct sockaddr *)&cli_addr, &clilen) < 0) {
        error("ERROR in UDP recvfrom");
    }

    printf("UDP message received: %s", buffer);

    if (sendto( sock ,"I got your message\n",19,
               0,(struct sockaddr *)&cli_addr, clilen) < 0) {
        error("ERROR in UDP sendto");
    }
}
