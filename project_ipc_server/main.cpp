#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <sys/ioctl.h>

#include <netinet/in.h>
#include <pthread.h>
#include <unistd.h>
#include <fcntl.h>

#include <iostream>

using namespace std;

void error(const char *msg)
{
    perror(msg);
    exit(1);
}

void* echo(int sockt)
{
    char buf[1024];
    int count;
    printf("Server is about to start reading...\n");

//    int flags = fcntl(sockt, F_GETFL, 0);
//    fcntl(sockt, F_SETFL, flags | O_NONBLOCK);

    count = (read(sockt, buf, 1023));

    if (count > 0)
    {
        printf("Server received '%s' from client\n", buf);
        printf("Server sending it back\n");
        write(sockt, buf, strlen(buf));
    }
    else
    {
        cout << "Error reading from socket: " << count << endl;
    }

    close(sockt);
}

int main(int argc, char *argv[])
{
     int sockfd, newsockfd, portno = 9999;
     // create a TCP/IP socket
     sockfd = socket(AF_INET, SOCK_STREAM, 0);
     if (sockfd < 0)
        error("ERROR opening socket");

     int flags = fcntl(sockfd, F_GETFL, 0);
     fcntl(sockfd, F_SETFL, flags | O_NONBLOCK);

     struct sockaddr_in serv_addr;
     // clear address structure
     bzero((char *) &serv_addr, sizeof(serv_addr));

     /* setup the host_addr structure for use in bind call */
     // server byte order
     serv_addr.sin_family = AF_INET;

     // automatically be filled with current host's IP address
     serv_addr.sin_addr.s_addr = INADDR_ANY;

     // port number to bind to
     serv_addr.sin_port = htons(portno);

     // This bind() call will bind  the socket to the current IP address on port
     if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0)
     {
         error("ERROR on binding");
     }

     // This listen() call tells the socket to listen to the incoming connections.
     // The listen() function places all incoming connection into a backlog queue
     // until accept() call accepts the connection.
     // Here, we set the maximum size for the backlog queue to 5.
     listen(sockfd, 5);

     cout << "Server is about to start accepting..." << endl;
     while (newsockfd = accept(sockfd, 0, 0))
     {
         cout << "Accepted new socket: " << newsockfd << endl;

         echo(newsockfd);

         cout << "Waiting for new client...\n" << endl;
     }

     return 0;
}
