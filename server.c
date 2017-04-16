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
      int tcp_sockfd, udp_sockfd, newsockfd, portno, max_fds, pid, n;
      socklen_t clilen;
      char buffer[256];
      struct sockaddr_in serv_addr, cli_addr;
      fd_set sock_set;

     if (argc < 2) {
         fprintf(stderr,"ERROR, no port provided\n");
         exit(1);
     }

     tcp_sockfd = socket(AF_INET, SOCK_STREAM, 0);

     if (tcp_sockfd < 0) {
         error("ERROR opening TCP socket");
     }

     bzero((char*)&serv_addr, sizeof(serv_addr));
     portno = atoi(argv[1]);

     serv_addr.sin_family = AF_INET;
     serv_addr.sin_addr.s_addr = INADDR_ANY;
     serv_addr.sin_port = htons(portno);

     if (bind(tcp_sockfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0) {
         error("ERROR on TCP binding");
     }

     listen(tcp_sockfd, 5);

     // create the udp socket
     udp_sockfd = socket(AF_INET, SOCK_DGRAM, 0);
     if (udp_sockfd < 0) error("ERROR opening UDP socket");

     if (bind(udp_sockfd, (struct sockaddr *)&serv_addr , sizeof(serv_addr)) < 0)
         error("ERROR on UDP binding");

     // clear out sock_set
     FD_ZERO(& sock_set);

     // set the max socket file descriptor number
     max_fds = MAX(tcp_sockfd, udp_sockfd) + 1;

     clilen = sizeof(cli_addr);

     while (1) {

       // add both file descriptors to the fd_set
       FD_SET(tcp_sockfd, &sock_set);
       FD_SET(udp_sockfd, &sock_set);

       // wait for one of the sockets to be ready
       if (select(max_fds, &sock_set, NULL, NULL, NULL) < 0) {
           error("ERROR on select");
       }


       // handle the tcp connection
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
               close(tcp_sockfd);
               close(udp_sockfd);
               serverReadWrite(newsockfd);
               exit(0);
           } else {
               close(newsockfd);
               signal(SIGCHLD, SIG_IGN);
           }
       }

       // handle the udp connection
       if (FD_ISSET(udp_sockfd, &sock_set)) {

           if (n < 0)
               error("ERROR on UDP recvfrom");

           pid = fork();

           if (pid < 0) {
               error("ERROR on TCP fork");
           }

           if (pid == 0) {
               close(tcp_sockfd);
               serverReadWriteUdp(udp_sockfd);
               exit(0);
           } else {
               signal(SIGCHLD, SIG_IGN);
           }
       }
     }

     close(udp_sockfd);
     close(tcp_sockfd);
     return 0;
}
