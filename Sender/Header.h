//
//  Header.h
//  
//
//  Created by Tyler Laracuente on 3/27/13.
//
//

#ifndef _Header_h
#define _Header_h
#define Q_SIZE 4

                              /* INCLUDES */ 
#include <semaphore.h>
#include <errno.h>      /* for errno */


                              /* VARIABLES */ 
char *queue[Q_SIZE+1];
int clientfd;

                              /* SEMAPHORES */
sem_t* lock;

                              /* STRUCTURES */

struct fileToSend {
    int fd;
    FILE *file;
};

struct pktData {
    int checksum;
    int size; 
    int seqNum;
    int isACK;
    char *line;
};




                               /* FUNCTIONS */

int open_clientfd(char *hostname, int port); /* External FD Opener */
void* send_file(void *args); /* External start routine for the sender thread. Sends lines to the packetwrapper and stores them in the window */
char* packetWrapper(void *args); /* Concatenates header info onto lines of the file */
void* clientReceiver(void *args); /*Thread to lisen for ACKs */
void resend(); /*Function to resend last packet for timeout or disorder*/




#endif
