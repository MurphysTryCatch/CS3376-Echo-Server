//*************************************************
//
//	Authors: 		Daniel Reiling,
//
//	File: 			log_s.c
//	professor: 		Dr. Belkoura
//	Group: 			Group #7
//
//	Program description: 	log server will accept incoming messages that
//							the echo_s.c server has recived and logs them
//							into a file echo.log.
//
//	Original Program:	http://www.linuxhowtos.org/C_C++/socket.htm
//
//*************************************************

#include <sys/types.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <netdb.h>
#include <stdio.h>
#include <time.h>
#include <arpa/inet.h>
#include <signal.h>
#include "client_functions.h"

int main(int argc, char *argv[])
{
   int sock, length, n, pid, port = 9999;
   socklen_t fromlen;
   struct sockaddr_in server;
   struct sockaddr_in from;
   char buf[1024];
   FILE *echop;
   time_t t = time(NULL);
   struct tm tm = *localtime(&t);

	//open and error check the socket
   sock=socket(AF_INET, SOCK_DGRAM, 0);
   if (sock < 0) error("Opening socket");

   //set the server address
   length = sizeof(server);
   bzero(&server,length);
   server.sin_family=AF_INET;
   server.sin_addr.s_addr=INADDR_ANY;
   server.sin_port=htons(port);

   //bind the server address and port number to the socket
   if (bind(sock,(struct sockaddr *)&server,length)<0)
       error("binding");

	//set the length of the address structure
   fromlen = sizeof(struct sockaddr_in);


   echop = fopen("echo.log", "w");
   //handle incoming connections indefinitaly
   while (1) {

	   //clear out the buffer
	   bzero(&buf, 1024);

	   //wait for an incoming message from the echo_s.c server
       n = recvfrom(sock,buf,1024,0,(struct sockaddr *)&from,&fromlen);
	   if (n < 0) error("recvfrom");

	   //create a child process to handle the logging of the message
	   pid = fork();

	   //error check the forking off of child process
	   if (pid < 0) {
		   error("ERROR on TCP fork");
	   }

	   if (pid == 0) {

		   //print out the buffer to the console
		   printf("UDP message received to log: %s\n", buf);

		   /* At this point the message recived is in the "buf" buffer
		   		with the length of the message in "n" for anyone that is
		   		handling writing that message to the echo.log file */
			char line[n];
			strncpy(line, buf, n);
			fprintf(echop, "%d-%d-%d %d:%d:%d ", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec);
			fprintf(echop, "%s was received from %s\n", line, inet_ntoa(from.sin_addr));


		   //send back an ack
	       n = sendto(sock,"Got your message\n",17,
	                  0,(struct sockaddr *)&from,fromlen);
	       if (n  < 0) error("sendto");

		   //close the socket and exit the child process
		   fclose(echop);
		   close(sock);
		   exit(0);
	   } else {
		   signal(SIGCHLD, SIG_IGN);
	   }

   }
   fclose(echop);
   return 0;
 }
