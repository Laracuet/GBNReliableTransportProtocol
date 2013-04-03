//
//  receiveACKs.c
//  
//
//  Created by Tyler Laracuente on 4/2/13.
//
//

#include <stdio.h>
#include <string.h>

#include "Header.h" 

#define PKT_SIZE 1024

void* receiveACKs(void *args) {
    
    //cast void* typed argument to our fileToSend struct
    struct fileToSend *data = (struct fileToSend*)args;
    
    //get the clientfd from the struct
    int clientfd = data->fd; 
    
    //create a buffer for the ACK we will receive and for the packet we will remove
    char packet[PKT_SIZE];
    char removedPacket[PKT_SIZE];
    char *check;
    
    
    //set the sequence counter for the 0th packet
    int sequenceCounter = 0; 
    
    //forever
    for (;;) {
        //cast the sequence number to a char
        char sequenceChar = '0'+ sequenceCounter;
        
        //wait for the ACK from stdin
        //SEGFAULT
        read(clientfd, packet, sizeof(packet));
        printf("read unblocked/"); 
        
        //set a 'check' string to check for:
        //isACK == 1
        strcat(check, "1");
        //the correct sequence number
        strcat(check, &sequenceChar);
        //checksum == 0
        strcat(check, "0"); 
        
        //if our header is good (first 3 chars match our check)
        if (strncmp(packet, check, 3)==0){
    
            //clear the memory for our packet buffer and pull the newest packet off of the queue.
            memset(removedPacket, '\0', sizeof(removedPacket));
            removeFromQ(&removedPacket);
            
            //test
            printf("%s", removedPacket);
    
            //write the newest packet to the client
            write(clientfd, removedPacket, sizeof(removedPacket));
            
            // unlock the semaphore
            sem_post(lock);
            
            //increment the sequence counter
            if (sequenceCounter == 10){
                sequenceCounter = 1;
            }
            else {
                sequenceCounter++;
            }

        }
    }
}