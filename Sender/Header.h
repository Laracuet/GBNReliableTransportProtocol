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

                              /* VARIABLES */ 
char *queue[Q_SIZE];

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
void* send_file(void *args); /* External start routine for the sender thread */
char* packetWrapper(void* args); /* Concatenates header info onto lines of the file */
void* receiveACKs(void *args); /* Thread to listen for ACKs and adjust semaphores */




#endif
