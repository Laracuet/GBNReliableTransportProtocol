#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <string.h>
#include <pthread.h>

#define PORT 5555
#define HOST_NAME "tylers-mac.bc.edu"
#define NUM_THREADS 2 
#define LINE_SIZE 256

struct fileToSend {
    int fd;
    FILE *file;
}; 

int open_clientfd(char *hostname, int port); /* External FD Opener */
void* send_file(void *args); /* External start routine for the sender thread */


int main(int argc, char *argv[]) {
	int clientfd;
    struct fileToSend fileData;
    void *args = (void *)&fileData;
    FILE *file = fopen("Reluctance.txt", "r");

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
    pthread_create(&thread[0], NULL, send_file, &fileData);
    //pthread_create(&thread[1], NULL, , )
    
    /* Wait for thread to complete */
    //pthread_join(thread[0], NULL);
    
    for(;;) {
        char line[LINE_SIZE];
        fgets(line, LINE_SIZE, stdin);
        int length = strlen(line);
        
        write(clientfd, line, length); 
    }
    

	close(clientfd);
    return 0; 
}
