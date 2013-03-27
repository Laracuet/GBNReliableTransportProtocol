#include <stdio.h>      /* for printf() and fprintf() */
#include <sys/socket.h> /* for socket() and bind() */
#include <arpa/inet.h>  /* for sockaddr_in and inet_ntoa() */
#include <stdlib.h>     /* for atoi() and exit() */
#include <string.h>     /* for memset() */
#include <unistd.h>     /* for close() */
#include <errno.h>      /* for errno */

#define ECHOMAX 256     /* Longest string to echo */


void receiveIt();
void sendIt();
void printIt(char *msg, int num);
void *ack_function(void *arg);
void *send_function(void *arg);

int sock;                        /* Socket */
struct sockaddr_in echoClntAddr; /* Requestor address */
char echoBuffer[ECHOMAX];        /* Buffer for echo string */
int recvMsgSize;                 /* Size of received message */
unsigned int cliAddrLen;         /* Length of incoming message */

void DieWithError(char *errorMessage);  /* External error handling function */

int main(int argc, char *argv[])
{
    struct sockaddr_in echoServAddr; /* Local address */
    unsigned short echoServPort;     /* Server port */
	pthread_t a_thread,b_thread;
	void *thread_result;
	int res;
	int firstTime = 1;

    if (argc != 2)         /* Test for correct number of parameters */
    {
        fprintf(stderr,"Usage:  %s <UDP SERVER PORT>\n", argv[0]);
        exit(1);
    }

    echoServPort = atoi(argv[1]);  /* First arg:  local port */

    /* Create socket for sending/receiving datagrams */
    if ((sock = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP)) < 0)
        DieWithError("socket() failed");
		
	struct timeval tv;
	
	tv.tv_sec = 3;  /* 30 Secs Timeout */
	tv.tv_usec = 0;  // Not init'ing this can cause strange errors
	/*
	 You can use the setsockopt function to set a timeout on receive operations:
	 
	 
	 SO_RCVTIMEO
	 
	 Sets the timeout value that specifies the maximum amount of time an input 
	 function waits until it completes. It accepts a timeval structure with 
	 the number of seconds and microseconds specifying the limit on how long 
	 to wait for an input operation to complete. If a receive operation has 
	 blocked for this much time without receiving additional data, it shall 
	 return with a partial count or errno set to [EAGAIN] or [EWOULDBLOCK] if 
	 no data is received. The default for this option is zero, which indicates 
	 that a receive operation shall not time out. This option takes a 
	 timeval structure. Note that not all implementations allow this option to be set (mac does).
	 */
	setsockopt(sock, SOL_SOCKET, SO_RCVTIMEO, (char *)&tv,sizeof(struct timeval));
	
    /* Construct local address structure */
    memset(&echoServAddr, 0, sizeof(echoServAddr));   /* Zero out structure */
    echoServAddr.sin_family = AF_INET;                /* Internet address family */
    echoServAddr.sin_addr.s_addr = htonl(INADDR_ANY); /* Any incoming interface */
    echoServAddr.sin_port = htons(echoServPort);      /* Local port */

    /* Bind to the local address */
    if (bind(sock, (struct sockaddr *) &echoServAddr, sizeof(echoServAddr)) < 0)
        DieWithError("bind() failed");
  	
	/* Set the size of the in-out parameter */
	cliAddrLen = sizeof(echoClntAddr);
	
	while (firstTime == 1){
		
		if ((recvMsgSize = recvfrom(sock, echoBuffer, ECHOMAX, 0,
									(struct sockaddr *) &echoClntAddr, &cliAddrLen)) < 0) //blocks for x seconds
			
		{
			/* error: recvfrom() would have timed out */
			if (errno == EWOULDBLOCK)  
				printf("still waiting\n");
		}	
		else
		{		
			printf("made first contact with requester\n");
			printIt(echoBuffer,recvMsgSize);
			firstTime = 0;
		}
	}
	
	printf("decide on window size, etc.\n");
	printf("now create a two threads to handle sending packets and receive acks from requestor\n");
		
	res = pthread_create(&a_thread, NULL, ack_function, NULL);  //thread for ack reads
	if (res != 0) {
		perror("Thread creation failed");
		(EXIT_FAILURE);
	}
	
	res = pthread_create(&b_thread, NULL, send_function, NULL);  //thread for apacket sends
	if (res != 0) {
		perror("Thread creation failed");
		(EXIT_FAILURE);
	}	
	
	res = pthread_join(a_thread, &thread_result);
    if (res != 0) {
        perror("Thread join failed");
        exit(EXIT_FAILURE);
    }
    printf("Thread done\n");
    return 0; 
}      

void *ack_function(void *arg) {
	receiveIt();
}


void *send_function(void *arg) {
	sendIt();
}
void printIt(char *msg, int num){
	char *p;
	int i=0;
	p = msg;
	while (i < num){
		printf("%c",*(p+i));
		i++;
	}
}

void sendIt(){
	int packets = 10;
	int count=0;
	char *msg = "this is the message\n";
	int size = strlen(msg);
	int ss;
	/* Set the size of the in-out parameter */
	//cliAddrLen = sizeof(echoClntAddr);
	//printf("size = %d\n",size);
	//printIt(msg,size);
	for (count = 0; count < packets; count++) 
	{
		if ((ss=sendto(sock, msg,size, 0, (struct sockaddr *) &echoClntAddr, sizeof(echoClntAddr))) != size){
			//printf("ss = %d\n",ss);
			DieWithError("sendto() failed");
		}
	}
	
	
}

void receiveIt(){
	
	for (;;) /* Run forever */
	{
	/* Block until receive message from a client or time out*/
	if ((recvMsgSize = recvfrom(sock, echoBuffer, ECHOMAX, 0,
	    (struct sockaddr *) &echoClntAddr, &cliAddrLen)) < 0)
		
	{
		/* error: recvfrom() would have timed out */
		if (errno == EWOULDBLOCK)  
			printf("nothing on the line\n");
	}	
	else
	 {		
		 printf("recieved from requestor %s\n", inet_ntoa(echoClntAddr.sin_addr));
		 printIt(echoBuffer, recvMsgSize);
		 printf("I can now check for ack sequence number and adjust window appropriately\n");
	 }
	}
}
