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
    
    
    
    
    //forever loop
    
        //wait semaphore to be posted by the sender thread

        //read from a timed socket
    
        //if the read fails, the timeout ocurred
    
        //if we receive something, check if it is an ACK
    
            //if it is an ACK, check the sequence number
    
                //if it is the right sequence number, then post the semaphore for the queue and print the data.
    
        //post a semaphore to unlock the sender (3 sems in all. 1 to lock the sender, 1 to lock the receiver, and 1 counter for the queue)
    
    
}