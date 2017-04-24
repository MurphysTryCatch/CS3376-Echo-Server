#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

#include "client_functions.h"

int main(int argc, char *argv[])
{
	//internal flag for connection type
	int isTcp = 1;

	//check for sufficient number of arguments
    if (argc < 3) {
       fprintf(stderr,"usage %s hostname port optional_flags\n", argv[0]);
       exit(0);
    }

	//check all the arguments for the udp flag
	for (int i = 3; i < argc; i++) {
		int comp = strcmp(argv[i], "-u");
		if (comp == 0) isTcp = 0;
	}

	if (isTcp == 1) {

		/* TCP connection */

	    int sockfd, portno, n;
	    struct sockaddr_in serv_addr;
	    struct hostent *server;
	    char buffer[256];

		//open the socket and get the port number
		portno = atoi(argv[2]);
	    sockfd = socket(AF_INET, SOCK_STREAM, 0);

		//error check the socket
	    if (sockfd < 0) {
	        error("ERROR opening socket");
	    }

		//check the hostname
	    server = gethostbyname(argv[1]);

		//error check the host name
	    if (server == NULL) {
	        fprintf(stderr,"ERROR, no such host\n");
	        exit(0);
	    }

		//set the server address
	    bzero((char*) &serv_addr, sizeof(serv_addr));
	    serv_addr.sin_family = AF_INET;
	    bcopy((char*)server->h_addr, (char*)&serv_addr.sin_addr.s_addr, server->h_length);
	    serv_addr.sin_port = htons(portno);

	    if (connect(sockfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0) {
	        error("ERROR connecting");
	    }

	    if (sendMessage(buffer, sockfd) < 0) {
	         error("ERROR writing to socket");
	    }

	    if (readMessage(buffer, sockfd) < 0) {
	         error("ERROR reading from socket");
	    }

	    printf("%s\n",buffer);
	    close(sockfd);

	} else if (isTcp == 0) {

		/* UDP connection */

		int sockfd, n;
	    unsigned int length;
	    struct sockaddr_in server, from;
	    struct hostent *hp;
	    char buffer[256];

	    // open the socket
	    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
	    if (sockfd < 0) error("ERROR opening UDP socket");

	    // set the server address
	    server.sin_family = AF_INET;
	    hp = gethostbyname(argv[1]);
	    if (hp==0) error("Unknown host");
	    bcopy((char *)hp->h_addr,
	         (char *)&server.sin_addr,
	          hp->h_length);
	    server.sin_port = htons(atoi(argv[2]));

	    length=sizeof(struct sockaddr_in);

	    // prompt user for message to send
	    printf("Please enter the message: ");
	    bzero(buffer,256);
	    fgets(buffer,255,stdin);

		char *pos;
		if ((pos=strchr(buffer, '\n')) != NULL)
		    *pos = '\0';
		else
    		error("ERROR removing the newline char from input");

	    // send the message to the server
	    n=sendto(sockfd,buffer,
	             strlen(buffer),0,(const struct sockaddr *)&server,length);
	    if (n < 0) error("Sendto");

	    // display the the message sent to the server
	    printf("\nHere is your message: %s\n", buffer);

	    // get response from server
	    n = recvfrom(sockfd,buffer,256,0,(struct sockaddr *)&from, &length);
	    if (n < 0) error("recvfrom");

	    // display the response to the user
	    write(1,buffer,n);

	    // close socket and exit program
	    close(sockfd);
	} else {

		error("ERROR could not determine socket type");
	}

    return 0;
}
