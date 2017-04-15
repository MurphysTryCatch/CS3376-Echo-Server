#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <string.h>


sig_atomic_t child_exit_status;
void clean_up_child_process (int signal_number)
{
        int status;                                             

        wait3(NULL, WNOHANG, NULL);

        child_exit_status = status;                             
}

int main ()
{
        pid_t child;
        int i;

        struct sigaction sigchld_action;                        
        memset (&sigchld_action, 0, sizeof (sigchld_action));
        sigchld_action.sa_handler = &clean_up_child_process;
        sigaction (SIGCHLD, &sigchld_action, NULL);

                printf("Now Forking \n");

                child = fork ();                                
                if (child > 0) {

                  sleep (3);                                    
                }
                else  {

                  exit (0);                                     
                }

        return 0;
}
