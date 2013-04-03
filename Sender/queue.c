//
//  queue.c
//  
//
//  Created by Tyler Laracuente on 4/2/13.
//
//

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>          /* exit */ 
#include <semaphore.h> 
#include <errno.h>
#include <string.h>
#include <fcntl.h>           /* For O_* constants */
#include <sys/stat.h>        /* For mode constants */

#include "Header.h"

int nextIn = Q_SIZE;
int nextOut = Q_SIZE;


void add2Q(char *newPacket) {
    
    queue[nextIn] = newPacket;
       sem_wait(lock);
    
    //printf("%s", queue[nextIn]);
    
    if(nextIn == 0)
        nextIn = Q_SIZE;
    else
        nextIn--;
    
}

void removeFromQ(char *packet) {
    char *sendPacket;
    
    //if there is an item on the queue, return it and replace it with '\0'
    if (queue[nextOut]){
        sendPacket = queue[nextOut];
        queue[nextOut] = '\0';
        
        
        if(nextOut == 0)
            nextOut = Q_SIZE;
        else
            nextOut--;
    }
    
    // if there are no items on the queue, throw an error
    else {
        printf("NO ITEMS IN QUEUE\n");
        *packet = '\0';
        exit(1); 
    }
    
    //return the packet
    strcpy(packet, sendPacket);
    
}

/*//Testing
int main() {
    printf("%d: Q_SIZE, proof that we read header file\n", Q_SIZE);
    
    //initializing semaphore
    lock = sem_open("/lock", O_CREAT, 0, 6);
    
    char *a = "Hey\n";
    char *b = "You!\n";
    char *c = "HEY\n";
    char *d = "YES YOU\n";
    char *e = "WHAT DO YOU WANT\n";
    char *f = "THIS IS ONE TOO MANY\n";
    char *g = "2\n";
    char *h = "3\n";
    char *i = "4\n";
    char *j = "5\n";
    
    add2Q(a);
    sleep(1); 
    add2Q(b);
    sleep(1); 
    add2Q(c);
    sleep(1); 
    add2Q(d);
    sleep(1); 
    add2Q(e);
    sleep(1);
    add2Q(f);
    sleep(1);
    add2Q(g);
    sleep(1);
    add2Q(h);
    sleep(1);
    add2Q(i);
    sleep(1);
    add2Q(j);
    
    //closing semaphore
    sem_close(lock);
    sem_unlink("/lock"); 
    
    return 0; 

}*/ 