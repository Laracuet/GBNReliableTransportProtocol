#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <pthread.h>

#define PORT 5556
#define HOST "Tylers-MacBook-Pro-2.local"
#define SIZE 1000000

void* clientReceiver(void* args); 

void error (const char *msg)
{
  perror (msg);
  exit (1);
}

// Take this getServerSocketfd function as-is
// Choose your port number (portno), back comes an fd
int getServerSocketfd(int portno)
{
  int sockfd;
  struct sockaddr_in serv_addr;

  sockfd = socket (AF_INET, SOCK_STREAM, 0);
  if (sockfd < 0)
    error ("ERROR opening socket");
  bzero ((char *) &serv_addr, sizeof (serv_addr));
  serv_addr.sin_family = AF_INET;
  serv_addr.sin_addr.s_addr = INADDR_ANY;
  serv_addr.sin_port = htons (portno);
  if (bind (sockfd, (struct sockaddr *) &serv_addr, sizeof (struct sockaddr_in)) < 0)
    error ("ERROR on binding");
  if (listen (sockfd, 20)) // 20 is number of outstanding connections to allow
    error("Error on socket listen");
  return sockfd;
}

int main (int argc, char *argv[]) {
    printf("Server Started\n"); 
	int serverSocket = getServerSocketfd(PORT);

	//opens a socket on the port and hangs until a client comes
    struct sockaddr_in cli_addr;
    socklen_t clilen = sizeof (struct sockaddr_in);
    
    // This next line blocks (waits) until a client connects
    int clientSocketfd = accept (serverSocket, (struct sockaddr *) &cli_addr, &clilen);
    printf("clilen = %d\n", clilen);
    if (clientSocketfd < 0)
        error ("ERROR on accept");
    
    void *args = (void *)&clientSocketfd;
    
    pthread_t thread[2];
     
    //Thread to receive packets
    //pthread_create(&thread[0], NULL, clientReceiver, args);
    //Thread to send ACKs
    //pthread_create();
    char *msg = "HEY";
    write(clientSocketfd, msg, sizeof(msg));
    printf("%s\n", msg); 

    close (clientSocketfd);
	close (serverSocket);
	return 0;
}
