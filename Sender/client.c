#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <string.h>
#include <pthread.h>
#include <fcntl.h>           /* For O_* constants */
#include <sys/stat.h>        /* For mode constants */



#include "Header.h" 

#define PORT 5556
#define HOST_NAME "tylers-mac.bc.edu"
#define NUM_THREADS 2 
#define LINE_SIZE 256

int main(int argc, char *argv[]) {
	int clientfd;
    struct fileToSend fileData;
    void *args = (void *)&fileData;
    FILE *file = fopen("Reluctance.txt", "r");
    
    //Clearing semaphores
    sem_close(lock);
    sem_unlink("/lock");
    
    //initializing semaphore
    lock = sem_open("/lock", O_CREAT, 0, Q_SIZE+1);


    if (file == NULL) {
        printf("Unable to open file.\n");
        exit(1); 
    }

	clientfd = open_clientfd(HOST_NAME, PORT);
	if (clientfd<0) {
		fprintf(stderr, "Unable to open port %d on server %s\n", PORT, HOST_NAME);
		exit(4);
	}
    
    fileData.file = file; 
    fileData.fd = clientfd; 
    
    pthread_t thread[NUM_THREADS];
    
    /* Launch Threads */
    printf("about to launch threads\n"); 
    //Thread to send packets
    pthread_create(&thread[0], NULL, send_file, &fileData);
    
    //Thread to receive ACKs 
    pthread_create(&thread[1], NULL, receiveACKs, &fileData);
    
    /* Wait for thread to complete */
    //pthread_join(thread[0], NULL);
    
    
    
    //If this is not here, the program freaks out...seriously, try it
    for(;;) {
    }
    
    //closing semaphore
    sem_close(lock);
    sem_unlink("/lock"); 
	
    //closing socket
    close(clientfd);
    return 0; 
}
