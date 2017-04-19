//*************************************************
//
//	Author: 		Daniel Reiling
//	File: 			udp_client.c
//	professor: 		Dr. Belkoura
//	Group: 			Group #7
//
//	File contents: UDP client to send a datagram to
//					a server and await the response
//
//	Original Program:	http://www.linuxhowtos.org/C_C++/socket.htm
//
//*************************************************
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include "client_functions.h"

int main(int argc, char *argv[])
{
   int sockfd, n;
   unsigned int length;
   struct sockaddr_in server, from;
   struct hostent *hp;
   char buffer[256];

   // check for all arguments
   if (argc != 3) {
       fprintf(stderr,"usage %s hostname port\n", argv[0]);
        exit(1);
   }

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

   // send the message to the server
   n=sendto(sockfd,buffer,
            strlen(buffer),0,(const struct sockaddr *)&server,length);
   if (n < 0) error("Sendto");

   // display the the message sent to the server
   printf("\nHere is your message: %s", buffer);

   // get response from server
   n = recvfrom(sockfd,buffer,256,0,(struct sockaddr *)&from, &length);
   if (n < 0) error("recvfrom");

   // display the response to the user
   write(1,buffer,n);

   // close socket and exit program
   close(sockfd);
   return 0;
}
