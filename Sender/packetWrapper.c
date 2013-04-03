//
//  packetWrapper.c
//  
//
//  Created by Tyler Laracuente on 3/26/13.
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

#define BUF_SIZE 100


char* packetWrapper(void* args){
    
    //casting the arguments back to the data struct
    struct pktData *newData = (struct pktData*)args;
    
    //Loading values from data struct
    int check = newData->checksum;
    //int check = checksum_generator();
    
    //Increment sequence number
    int seq = newData->seqNum;
    
    //Might be a constant 0
    //int ACK = newData->isACK;
    int ACK = 0; 
    
    //cast integer values from data struct to a char buffer
    char checksum = '0' + check;
    char seqNum = '0' + seq;
    char isACK = '0' + ACK;
    
    //create a buffer and concatenate all header values into it, finally including the line as the body. 
    char line[4000];
    memset(line, '\0', sizeof(line));
    
    //concatenating the address at isACK takes all values from the struct into the header. 
    strcat(line, &isACK);
    strcat(line, "\n"); 
    strcat(line, newData->line);
    
    char *l = line;
    return l;
}
