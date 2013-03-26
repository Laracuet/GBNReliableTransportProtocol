//
//  send_file.c
//  
//
//  Created by Tyler Laracuente on 3/25/13.
//
//

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <string.h>
#include <pthread.h>

#define LINE_SIZE 256

struct fileToSend {
    int fd;
    FILE *file;
};

char* packetWrapper(void* args);

void* send_file(void *d) {
    
    //cast void* typed argument to our fileToSend struct
    struct fileToSend *data = (struct fileToSend*)d;

    char line[LINE_SIZE];
    int clientfd = data->fd;
    FILE *file = data->file;
    
    while(fgets(line, LINE_SIZE, file) != NULL){
        int length = strlen(line);
        printf("%s", line);
        write(clientfd, line, length);
        sleep(1);
    }
}