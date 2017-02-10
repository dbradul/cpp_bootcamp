#include <iostream>

#include <sys/mman.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/socket.h>

#include <pthread.h>
#include <fcntl.h>
#include <semaphore.h>
#include <netinet/in.h>
#include <netdb.h>

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

using namespace std;

namespace shared_memory {

    int run()
    {
        pthread_mutexattr_t attributes;

        pthread_mutexattr_init(&attributes);
        pthread_mutexattr_setpshared(&attributes, PTHREAD_PROCESS_SHARED);

        int handle = shm_open("/shm42", O_CREAT | O_RDWR, 0777);
        ftruncate(handle, 2048*sizeof(int));
        char *memory = (char *)mmap(0, 2048*sizeof(int),
                                    PROT_READ | PROT_WRITE,
                                    MAP_SHARED, handle, 0);

        // mutex share
        pthread_mutex_t *mutex = (pthread_mutex_t*)memory;
        pthread_mutex_init(mutex, &attributes);

        pthread_mutexattr_destroy(&attributes);

        // variable share
        int *count = (int*)(memory + sizeof(pthread_mutex_t));
        *count = 0;
        printf("Initial count = %d\n", *count);

        pid_t pid = fork();
        if(pid == 0)
        {
           pthread_mutex_lock(mutex);
           (*count)++;
           printf("Child process increased the count to %d\n", *count);
           pthread_mutex_unlock(mutex);

           return 0;
        }

        else
        {
           int status;
           // waiting for the child process to finish
           waitpid(pid, &status, 0);
           pthread_mutex_lock(mutex);
           (*count)++;
           printf("Parent process increased the count to %d\n", *count);
           pthread_mutex_unlock(mutex);
        }

        munmap(memory, 2048*sizeof(int));
        shm_unlink("/shm42");

        return 0;
    }

}


namespace shared_memory_with_semaphores {

    int run()
    {
        pthread_mutexattr_t attributes;

        pthread_mutexattr_init(&attributes);
        pthread_mutexattr_setpshared(&attributes, PTHREAD_PROCESS_SHARED);

        int handle = shm_open("/shm42", O_CREAT | O_RDWR, 0777);
        ftruncate(handle, 2048*sizeof(int));
        char *memory = (char *)mmap(0, 2048*sizeof(int),
                                    PROT_READ | PROT_WRITE,
                                    MAP_SHARED, handle, 0);

        // sema share
        sem_t *semaphore_client =  sem_open("/sema_client", O_CREAT,  0777, 0);
        sem_t *semaphore_server =  sem_open("/sema_server", O_CREAT,  0777, 0);

        sem_post(semaphore_client);

        // variable share
        int *count = (int*)(memory/* + sizeof(pthread_mutex_t)*/);
        *count = 0;
        printf("Initial count = %d\n", *count);

        pid_t pid = fork();
        if(pid == 0)
        {
            while (*count<100)
            {
                sem_wait(semaphore_server);

                (*count)++;
                printf("Child process increased the count to %d\n", *count);

                sem_post(semaphore_client);
            }

            sem_close(semaphore_client);
            sem_unlink("/sema_client");

            return 0;
        }

        else
        {
           while (*count<100)
           {
               sem_wait(semaphore_client);

               (*count)++;
               printf("Parent process increased the count to %d\n", *count);

               sem_post(semaphore_server);
           }

           sem_close(semaphore_server);
           sem_unlink("/sema_server");

        }

        munmap(memory, 2048*sizeof(int));
        shm_unlink("/shm42");

        return 0;
    }

}



namespace sockets {



    void error(const char *msg)
    {
        perror(msg);
        exit(1);
    }

    void* echo(void* param)
    {
        char buf[1024];
        int count;
        //pthread_detach(pthread_self());
        int* p_s = (int*)param;
        int sockt = *p_s;
        while( count = read(sockt, buf, 1023) > 0 ) {
            printf("Server received '%s'\n", buf);
            printf("Server sending it back\n");
            write(sockt, buf, strlen(buf));
        }

        close(sockt);
    }


    int run()
    {
        pid_t pid = fork();
        if(pid == 0)
        {
            // client-side

            int sockfd, portno = 9999;
            struct sockaddr_in serv_addr;
            sockfd = socket(AF_INET, SOCK_STREAM, 0);
            if (sockfd < 0)
            {
                error("ERROR opening socket");
            }
            memset(&serv_addr, 0, sizeof(serv_addr));
            serv_addr.sin_family = AF_INET;
            serv_addr.sin_addr.s_addr = INADDR_ANY;//inet_addr("127.0.0.1");
            serv_addr.sin_port = htons(portno);

            if (connect(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) == 0)
            {
                printf("Client sending 'hello server!' to server\n");
                char buf[1024];
                strncpy(buf, "hello server!", 20);
                write(sockfd, buf, strlen(buf));
                int count = read(sockfd, buf, 1024);
                printf("Got echo of '%s' from server\n", buf);
                shutdown(sockfd, SHUT_RDWR);
            }
            else
            {
                error("ERROR connecting");
            }
            return 0;
        }

        else
        {
            // server-side

            int sockfd, newsockfd, portno = 9999;
            // create a TCP/IP socket
            sockfd = socket(AF_INET, SOCK_STREAM, 0);


            ///fcntl(sockfd, F_SETFL, O_NONBLOCK);

            if (sockfd < 0)
            {
               error("ERROR opening socket");
            }

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
            listen(sockfd,5);
            pthread_t t[5];
            int i=0;

            while (newsockfd = accept(sockfd, 0, 0))
            {
//                pthread_t t;
                pthread_create(&t[i++], 0, echo, (void*)newsockfd);
            }

            for(int i = 0; i<5; ++i)
            {
                pthread_join(t[i], nullptr);
            }

            return 0;
        }


    }

}


int main(int argc, char *argv[])
{
//    {
//        using namespace shared_memory;

//        run();
//    }

//    {
//        using namespace shared_memory_with_semaphores;

//        run();
//    }

    {
        using namespace sockets;

        run();
    }

    return 0;
}
