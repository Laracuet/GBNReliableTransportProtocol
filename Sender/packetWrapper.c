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

#define cast_size 100


char* packetWrapper(void* args){
    
    //casting the arguments back to the data struct
    struct pktData *newData = (struct pktData*)args;
    
    //Loading values from data struct
    int check = newData->checksum;
    int siz = newData->size;
    int seq = newData->seqNum;
    int ACK = newData->isACK;
    
    //cast integer values from data struct to strings
    char checksum = check;
    char size = siz;
    char seqNum = seq;
    char isACK = ACK;
    
    //create a buffer and concatenate all header values into it, finally including the line as the body. 
    char line[4000];
    memset(line, '\0', sizeof(line));
    
    strcpy(line, &checksum);
    strcat(line, &size);
    strcat(line, &seqNum);
    strcat(line, &isACK);
    strcat(line, newData->line);
    
    char *l = line;
    return l;
}
