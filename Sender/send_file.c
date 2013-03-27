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

#include "Header.h"

#define LINE_SIZE 256

void* send_file(void *d) {
    
    //setting counter for sequence number setting during while loop
    int i=-1;
    
    //cast void* typed argument to our fileToSend struct
    struct fileToSend *data = (struct fileToSend*)d;

    char line[LINE_SIZE];
    int clientfd = data->fd;
    FILE *file = data->file;
    
    while(fgets(line, LINE_SIZE, file) != NULL){
        int length = strlen(line);
        struct pktData newPacket;
        void *args = (void *)&newPacket;
        
        //set up circular seq numbers and a better way to set the ACK up 
        newPacket.checksum = 0; //TEST VALUE
        newPacket.size = 0; //TEST VALUE
        newPacket.seqNum = i++; //TEST VALUE
        newPacket.isACK = 0; 
        newPacket.line = line; 
        
        char *fullpkt = packetWrapper(args); 
        if(!*fullpkt){
            
            //WRITE AN ERROR CHECK
            printf("ERROR"); 
        }
        
        //Debug print
        printf("%s", fullpkt);
        
        
        write(clientfd, fullpkt, length);
        sleep(1);
    }
}