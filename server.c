#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/stat.h>
#include <fcntl.h>

#define PORT 5555
#define HOST "tylers-mac.bc.edu"
#define SIZE 1000000

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
	int serverSocket = getServerSocketfd(PORT);

	//opens a socket on the port and hangs until a client comes
	while(1){
		struct sockaddr_in cli_addr;
		socklen_t clilen = sizeof (struct sockaddr_in);
		// This next line blocks (waits) until a client connects
			printf("clilen = %d\n", clilen);
		int clientSocketfd = accept (serverSocket, (struct sockaddr *) &cli_addr, &clilen);
			printf("clilen = %d\n", clilen);
		if (clientSocketfd < 0)
			error ("ERROR on accept");
	
#define BUFLEN 256
		while(1){	
			char buffer[BUFLEN];
			char page[BUFLEN];
			char contentType[BUFLEN];
			char pathBuff[BUFLEN];
			char header_out[BUFLEN];
			char *fileExt;
			struct stat fileStat;
			int fileSize = 0;
			int notFound = 0;		
		
			//reads the packet from the client and stores it in a buffer
			int n = read (clientSocketfd, buffer, BUFLEN - 1);	

			if (n < 0) {
				perror ("ERROR reading from socket");
				break;
			}
			buffer[n] = '\0';
			printf("%s", buffer);//debugging   		

            //if there is no adress, default.html can be assumed
			if(!strncmp(buffer, "GET / HTTP/1.1", 14)){
				strcpy(page, "default.html\0");
				printf ("Here is the page name: [%s]\n", page);  
			}

			//otherwise, parse the name of the requested page from the GET line
			else {
				char *afterSlash = (strchr(buffer, '/')+1);
        			char *spaceAfter = strstr(buffer, " HTTP");

        			if (afterSlash && spaceAfter && (afterSlash < spaceAfter) && (!strncmp(spaceAfter+1, "HTTP/", 5))) {
					int length = spaceAfter - afterSlash;
                			strncpy(page, afterSlash, length);
					page[length] = '\0'; 

                                }

				printf("\nHere is the page name: [%s]\n", page);//debugging
			}
			printf("Host: [%s] at Port: [%d]\n", HOST, PORT);//debugging

			//set up the file path
			sprintf(pathBuff, "WebPages/%s", page);
			printf("Page at path: [%s]\n", pathBuff);

			//Use stat() to extract file data, explicitly, the size
			if (stat(pathBuff, &fileStat) < 0) {
				printf("failed STAT");
				strcpy(pathBuff, "WebPages/NOTFOUND"); 
			}
			else {
				fileSize = fileStat.st_size;
				printf("size of the file is: [%d]\n", fileSize); 
						
				//then, scan the page name for the file ending and pull the extension						
				char *ext = strrchr(page, '.');
				fileExt = strndup(ext, 5);	
						
				//check the file extension and assign the appropriate content-type
				if (!strcmp(fileExt, ".html"))
					strcpy(contentType, "text/html; charset=UTF-8");
				else if (!strcmp(fileExt, ".jpg"))
					strcpy(contentType, "image/jpg");
				else if (!strcmp(fileExt, ".png"))
					strcpy(contentType, "image/png");
				else if(!strcmp(fileExt, ".gif"))
					strcpy(contentType, "image/gif");
				else {
					strcpy(contentType, "unknown"); 
				}
				printf("Here is the file extension: [%s]\n", contentType);//debugging
			
			
				//create the header with the file size and content-types found earlier. Write it to the client
				sprintf(header_out, "HTTP/1.1 200 OK\r\nContent-Length: %d\r\nContent-Type: %s\n\n", fileSize, contentType);
				printf("%s\n", header_out);
				n = write(clientSocketfd, header_out, strlen(header_out));
	
				if (n < 0) {
					perror ("ERROR writing to socket");
        	        	 	break; 		
				}	
			}

			//open the page
        	        int requestedPageFD = open(pathBuff, O_RDONLY);
        	        if (requestedPageFD < 0){
        	                perror("Could not open input file");
				break;
			}
		
			//ready the buffer for the file transfer
			char *writeBuff = malloc(SIZE); 
			if (!buffer){
				fprintf(stderr, "Unable to allocate buffer memory\n");
				break;
			}
			ssize_t bytesRead;

			//while bytes are still coming in, write them to the client
			while((bytesRead = read(requestedPageFD, writeBuff, SIZE))>0){
				ssize_t bytesWritten = write(clientSocketfd, writeBuff, bytesRead);
			
				if (bytesWritten ==-1){
					perror("error writing file");
					break;
				}

				if (bytesRead != bytesWritten)
					fprintf(stderr, "Warning: %d bytes read, but only %d bytes written", (int)bytesRead, (int)bytesWritten);
			}
			free(writeBuff);			

			if (bytesRead == -1){ 
				perror("error reading file");
				break;
			}
			
			//close the requested page
			close (requestedPageFD);
			break;
		}
		//errors break to here, killing that client's connection, but keeping the server up
		close (clientSocketfd); 
	}
	close (serverSocket);
	return 0;
}
