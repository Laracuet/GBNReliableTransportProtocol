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
    
    //cast void* typed argument to our fileToSend struct
    struct fileToSend *data = (struct fileToSend*)d;
    
    //create a buffer for the line we will pull from the file and for the full packet we will pull off of the queue
    char line[LINE_SIZE];
    char removedPacket[LINE_SIZE];
    
    //pull the file descriptor and the file from our arguments
    int clientfd = data->fd;
    FILE *file = data->file;
    
    //set a counter to help with sequence numbers 
    int sequenceCounter = 0;
   
    //while there are still lines in the file
    while(fgets(line, LINE_SIZE, file) != NULL){
        
        //create a struct to send to the packet wrapper
        struct pktData newPacket;
        void *args = (void *)&newPacket;
        
        /* WRITE THIS */ 
        //Use algorithm to decide checksum
        //newPacket.checksum; //TEST VALUE
        
        //set the sequence number in the packet struct 
        if (sequenceCounter == 10){
            newPacket.seqNum = 0;
            sequenceCounter = 1;
        }
        else {
            newPacket.seqNum = sequenceCounter;
            sequenceCounter++;
        }
        
        //add the data to the packet struct
        newPacket.line = line; 
        
        //send the packet info to the packet creation function
        char *fullpkt = packetWrapper(args);
        
        //check to see the packet came back correctly 
        if(!*fullpkt){
            //WRITE AN ERROR CHECK
            printf("ERROR"); 
        }

        //add the newly made packet to our send queue and increment the semaphore
        add2Q(fullpkt);
        
        //clear the memory for our packet buffer and pull the newest packet off of the queue. 
        memset(removedPacket, '\0', sizeof(removedPacket));
        removeFromQ(&removedPacket);
        printf("%s", removedPacket);
        
        //write the newest packet to the client 
        write(clientfd, removedPacket, sizeof(removedPacket));
        sleep(1);
    }
}