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

int sendToLog (const char *buf) {

	int sockfd, n;
	unsigned int length;
	struct sockaddr_in server, from;
	struct hostent *hp;

	// open the socket
	sockfd = socket(AF_INET, SOCK_DGRAM, 0);
	if (sockfd < 0) error("ERROR opening UDP socket");

	// set the server address
	server.sin_family = AF_INET;
	hp = gethostbyname("localhost");
	if (hp==0) error("Unknown host");
	bcopy((char *)hp->h_addr,
		 (char *)&server.sin_addr,
		  hp->h_length);
	server.sin_port = htons(9999);

	length=sizeof(struct sockaddr_in);

	// send the message to the server
	n=sendto(sockfd,buf,
			 strlen(buf),0,(const struct sockaddr *)&server,length);
	if (n < 0) error("Sendto");

	close(sockfd);
	return 1;

}

void serverReadWrite(int sock) {
	char buffer[256];
	bzero(buffer, 256);

	if (read(sock, buffer, 255) < 0) {
        error("ERROR reading from socket");
    }

    //print the message to the server console
    printf("TCP message received: %s", buffer);

	//send the message to the log server
	if (sendToLog(buffer) < 0) error("ERROR sending to log server");

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

	//send the message to the log server
	if (sendToLog(buffer) < 0) error("ERROR sending to log server");

    if (sendto( sock ,"I got your message\n",19,
               0,(struct sockaddr *)&cli_addr, clilen) < 0) {
        error("ERROR in UDP sendto");
    }
}
