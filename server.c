#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include<signal.h>
#include <unistd.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>

#include "server_functions.h"

int main(int argc, char *argv[])
{
     int sockfd, newsockfd, portno, n, pid;
     socklen_t clilen;
     char buffer[256];
     struct sockaddr_in serv_addr, cli_addr;

     if (argc < 2) {
         fprintf(stderr,"ERROR, no port provided\n");
         exit(1);
     }

     sockfd = socket(AF_INET, SOCK_STREAM, 0);

     if (sockfd < 0) {
        error("ERROR opening socket");
     }

     bzero((char*)&serv_addr, sizeof(serv_addr));
     portno = atoi(argv[1]);

     serv_addr.sin_family = AF_INET;
     serv_addr.sin_addr.s_addr = INADDR_ANY;
     serv_addr.sin_port = htons(portno);

     if (bind(sockfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0) {
        error("ERROR on binding");
     }

     listen(sockfd, 5);
     clilen = sizeof(cli_addr);

     while (1) {
        newsockfd = accept(sockfd, (struct sockaddr*)&cli_addr, &clilen);

        if (newsockfd < 0) {
            error("ERROR on accept");
        }

        pid = fork();

        if (pid < 0) {
            error("ERROR on fork");
        }

        if (pid == 0) {
            close(sockfd);
            serverReadWrite(newsockfd);
            exit(0);
        } else {
            close(newsockfd);
            signal(SIGCHLD, SIG_IGN);
        }
     }
     close(sockfd);
     return 0; 
}