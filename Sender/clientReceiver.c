//
//  clientReceiver.c
//  
//
//  Created by Tyler Laracuente on 4/8/13.
//
//

#include <stdio.h>
#include <string.h>

#include "Header.h"

#define BUFLEN 256

void* clientReceiver(void* d) {
    
    //cast void* argument to an int for our socket fd
    struct fileToSend *data = (struct fileToSend*)d;
    
    int clientSocketfd = data->fd;
    
    //TEST: set the seqNumber to 0 to start
    int seqNumber = 0; //FIX: make a global sequence number variable in a header file
    
    while(1){
        char buffer[BUFLEN];
        char seqCompare[BUFLEN];
        
        //TEST
        //printf("Current Sequence Number is: %d\n", seqNumber);
        
        //reads the packet from the client and stores it in a buffer
        int n = read (clientSocketfd, buffer, BUFLEN - 1);
        
        buffer[n] = '\0';
        printf("%s\n", buffer);//debugging
    
        //If what we receive is not a packet, print so, and break the connection
        /*if(strncmp(buffer, "0", 1) != 0){
            printf ("Packet is not correct: [%s]\n", buffer);
            break;
        }*/
    
        //otherwise, check the sequence number of the packet
       // else {
        
            //convert the seqNumber int to a char
            //FIX DO THIS LIKE THE OTHER PLACES
            char seq = '0' + seqNumber;
        
            //clear the seqCompare buffer
            memset(seqCompare, '\0', sizeof(seqCompare));
        
            strcpy(seqCompare, "0");
            strcat(seqCompare, &seq);
        
            //if the sequence number is incorrect, change nothing
            if(strncmp(seqCompare, buffer, 2) != 0){
                printf("Sequence number is incorrect, resending last ACK\n");
            }
            //otherwise, increment the global sequence number
            else {
               seqNumber++;
                printf("Good Seq\n");
            }
           
           //FIX: in any case, resend the appropriate ACK by unlocking the sender thread
       // }
    }
    return 0; 
}
