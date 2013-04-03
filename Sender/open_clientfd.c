
//  open_clientfd.c


#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <string.h>

int open_clientfd(char *hostname, int port) {
	int clientfd;
	struct hostent *hp;
	struct sockaddr_in *serveraddr = calloc(1, sizeof(struct sockaddr_in)); // use calloc, need zero'd
    
	// ** create socket
	if ((clientfd = socket(PF_INET, SOCK_DGRAM, 0))<0) {
		fprintf(stderr, "Error creating socket\n");
		free(serveraddr);
		return -1;
	}
    
	// ** Fill in the server's IP address and port
	if (!(hp = gethostbyname(hostname))) {
		fprintf(stderr, "Unable to get IP address of host %s\n", hostname);
		free(serveraddr);
		return -1;
	}
	serveraddr->sin_family = PF_INET;
	serveraddr->sin_port = htons(port);
    
	bcopy((char *)hp->h_addr_list[0],
	      (char *)&serveraddr->sin_addr.s_addr, hp->h_length);
    
	// ** Finally, establish the connection
	if (connect(clientfd, (struct sockaddr *)serveraddr, sizeof(struct sockaddr_in)) < 0) {
		fprintf(stderr, "Unable to connect to server %s\n", hostname);
		free(serveraddr);
		return -1;
	}
	free(serveraddr);
	return clientfd; // success
} 


