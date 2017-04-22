// Written by Matthew Dumler, Samuel Britton, and Keaton Khonsari

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <netinet/in.h>

#include "server_functions.h"
#define MAX(X, Y) (((X) > (Y)) ? (X) : (Y))

int main(int argc, char *argv[])
{
      int tcp_sockfd, tcp_sockfd2, tcp_sockfd3, udp_sockfd,udp_sockfd2, udp_sockfd3, newsockfd, portno, max_fds, pid, n;
     int portno2 = -1;
     int portno3 = -1;
     socklen_t clilen;
      char buffer[256];
      struct sockaddr_in serv_addr, cli_addr;
      fd_set sock_set;

     if (argc < 2) {
         fprintf(stderr,"ERROR, no port provided\n");
         exit(1);
     }
     tcp_sockfd= socket(AF_INET, SOCK_STREAM, 0);
     //collects additional ports if need be
     if(argc>2){
     	portno2=atoi(argv[2]);
       tcp_sockfd2 = socket(AF_INET, SOCK_STREAM, 0);
     }
     if(argc>3){
        portno3=atoi(argv[3]);
     	tcp_sockfd3= socket(AF_INET, SOCK_STREAM, 0);
     }
     if (tcp_sockfd < 0) {
         error("ERROR opening TCP socket");
     }
     if (tcp_sockfd2 < 0) {
         error("ERROR opening TCP socket");
     }
     if (tcp_sockfd3 < 0) {
         error("ERROR opening TCP socket");
     }

     bzero((char*)&serv_addr, sizeof(serv_addr));
     portno = atoi(argv[1]);


     serv_addr.sin_family = AF_INET;
     serv_addr.sin_addr.s_addr = INADDR_ANY;
     serv_addr.sin_port = htons(portno);
     //bind all the tcp ports
     if (bind(tcp_sockfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0) {
         error("ERROR on TCP binding first port");
     }
     if(portno2!=-1){
     	serv_addr.sin_port =htons( portno2);
     	if(bind(tcp_sockfd2, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) <0)
		error("Error on TCP binding second port");
     }

     if(portno3!=-1){
     	serv_addr.sin_port = htons(portno3);
     	if(bind(tcp_sockfd3, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) <0)
		error("Error on TCP binding third port");
     }
     //Listens on the tcp sockets
     listen(tcp_sockfd, 5);
     if(portno2!=-1)
     	listen(tcp_sockfd2,5);
     if(portno3!=-1)
     	listen(tcp_sockfd3,5);
     // create the udp sockets
     udp_sockfd = socket(AF_INET, SOCK_DGRAM, 0);
     if (udp_sockfd < 0) error("ERROR opening UDP socket");
     if(portno2!=-1){
     	udp_sockfd2= socket(AF_INET, SOCK_DGRAM, 0);
        if (udp_sockfd2< 0) error("ERROR opening UDP socket");
     }
     if(portno3!=-1){
     	udp_sockfd3= socket(AF_INET, SOCK_DGRAM, 0);
        if (udp_sockfd3< 0) error("ERROR opening UDP socket");
     }
     //binds all three UDP sockets
     serv_addr.sin_port = htons(portno);
     if (bind(udp_sockfd, (struct sockaddr *)&serv_addr , sizeof(serv_addr)) < 0)
         error("ERROR on UDP binding");
	     
     if(portno2!=-1){
	     serv_addr.sin_port = htons(portno2);
	     if (bind(udp_sockfd2, (struct sockaddr *)&serv_addr , sizeof(serv_addr)) < 0)
		 error("ERROR on UDP binding");
     }
     if(portno3!=-1){
	     serv_addr.sin_port = htons(portno3);
	     if (bind(udp_sockfd3, (struct sockaddr *)&serv_addr , sizeof(serv_addr)) < 0)
		 error("ERROR on UDP binding");
     }
     // clear out sock_set
     FD_ZERO(& sock_set);

     // set the max socket file descriptor number
     max_fds = MAX(tcp_sockfd, udp_sockfd);
     if(portno2!=-1){
     	max_fds = MAX(max_fds, tcp_sockfd2);
     	max_fds = MAX(max_fds, udp_sockfd2);
     }
     if(portno3!=-1){
     	max_fds = MAX(max_fds, tcp_sockfd3);
     	max_fds = MAX(max_fds, udp_sockfd3);
     }
     max_fds++;
     clilen = sizeof(cli_addr);

     while (1) {

       // add all file descriptors to the fd_set
       FD_SET(tcp_sockfd, &sock_set);
       FD_SET(udp_sockfd, &sock_set);
       if(portno2!=-1){
       	FD_SET(tcp_sockfd2, &sock_set);
        FD_SET(udp_sockfd2, &sock_set);
       }
       if(portno3!=-1){
       	FD_SET(tcp_sockfd3, &sock_set);
       	FD_SET(udp_sockfd3, &sock_set);
	}
       // wait for one of the sockets to be ready
       if (select(max_fds, &sock_set, NULL, NULL, NULL) < 0) {
           error("ERROR on select");
       }
       // handle the tcp connection of the first port
       if (FD_ISSET(tcp_sockfd, &sock_set)) {
           newsockfd = accept(tcp_sockfd, (struct sockaddr*)&cli_addr, &clilen);

           if (newsockfd < 0) {
               error("ERROR on TCP accept");
           }

           pid = fork();

           if (pid < 0) {
               error("ERROR on TCP fork");
           }

           if (pid == 0) {
		close(udp_sockfd);
		close(tcp_sockfd);
		if(portno2!=-1){
		close(udp_sockfd2);
		close(tcp_sockfd2);
		}
		if(portno3!=-1){
		close(udp_sockfd3);
		close(tcp_sockfd3);
		}
               serverReadWrite(newsockfd);
               exit(0);
           } else {
               close(newsockfd);
               signal(SIGCHLD, SIG_IGN);
           }
       }

       // handle the udp connection of the first port
       if (FD_ISSET(udp_sockfd, &sock_set)) {

           if (n < 0)
               error("ERROR on UDP recvfrom");

           pid = fork();

           if (pid < 0) {
               error("ERROR on UDP fork");
           }

           if (pid == 0) {
		close(tcp_sockfd);
		if(portno2!=-1){
		close(udp_sockfd2);
		close(tcp_sockfd2);
		}
		if(portno3!=-1){
		close(udp_sockfd3);
		close(tcp_sockfd3);
		}
               serverReadWriteUdp(udp_sockfd);
               exit(0);
           } else {
               signal(SIGCHLD, SIG_IGN);
           }
       }
       //handles connections if more than one port is specified
       if(portno2!=-1){
       		//Handles the tcp of port 2
	       if (FD_ISSET(tcp_sockfd2, &sock_set)) {
		   newsockfd = accept(tcp_sockfd2, (struct sockaddr*)&cli_addr, &clilen);

		   if (newsockfd < 0) {
		       error("ERROR on TCP accept");
		   }

		   pid = fork();

		   if (pid < 0) {
		       error("ERROR on TCP fork");
		   }

		   if (pid == 0) {
			close(udp_sockfd);
			close(tcp_sockfd);
			if(portno2!=-1){
			close(udp_sockfd2);
			close(tcp_sockfd2);
			}
			if(portno3!=-1){
			close(udp_sockfd3);
			close(tcp_sockfd3);
			}
		       
		       serverReadWrite(newsockfd);
		       exit(0);
		   } else {
		       close(newsockfd);
		       signal(SIGCHLD, SIG_IGN);
		   }
       		}
		//handles udp for port 2
	       if (FD_ISSET(udp_sockfd2, &sock_set)) {

		   if (n < 0)
		       error("ERROR on UDP recvfrom");

		   pid = fork();

		   if (pid < 0) {
		       error("ERROR on TCP fork");
		   }

		   if (pid == 0) {
			close(udp_sockfd);
			close(tcp_sockfd);
			close(tcp_sockfd2);
			if(portno3!=-1){
			close(udp_sockfd3);
			close(tcp_sockfd3);
			}
		       serverReadWriteUdp(udp_sockfd2);
		       exit(0);
		   } else {
		       signal(SIGCHLD, SIG_IGN);
		   }
	       }

       }
       //handling connections on a third port
       if(portno3!=-1){
       		//Handles the tcp of port 3
	       if (FD_ISSET(tcp_sockfd3, &sock_set)) {
		   newsockfd = accept(tcp_sockfd3, (struct sockaddr*)&cli_addr, &clilen);

		   if (newsockfd < 0) {
		       error("ERROR on TCP accept");
		   }

		   pid = fork();

		   if (pid < 0) {
		       error("ERROR on TCP fork");
		   }

		   if (pid == 0) {
			close(udp_sockfd);
			close(tcp_sockfd);
			close(udp_sockfd2);
			close(tcp_sockfd2);
			close(udp_sockfd3);
			close(tcp_sockfd3);
		       serverReadWrite(newsockfd);
		       exit(0);
		   } else {
		       close(newsockfd);
		       signal(SIGCHLD, SIG_IGN);
		   }
       		}
		//handles udp for port 3
	       if (FD_ISSET(udp_sockfd3, &sock_set)) {

		   if (n < 0)
		       error("ERROR on UDP recvfrom");

		   pid = fork();

		   if (pid < 0) {
		       error("ERROR on TCP fork");
		   }

		   if (pid == 0) {
			close(udp_sockfd);
			close(tcp_sockfd);
			close(udp_sockfd2);
			close(tcp_sockfd2);
			close(tcp_sockfd3);
		       serverReadWriteUdp(udp_sockfd3);
		       exit(0);
		   } else {
		       signal(SIGCHLD, SIG_IGN);
		   }
	       }

       }
     }

     close(udp_sockfd);
     close(tcp_sockfd);
     return 0;
}
