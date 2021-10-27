#include "defs.h"
#include "server.h"
#include "procesos.h"
#include "thread.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/socket.h>
#include <sys/types.h>

GPSData gpsData;
int serverFd, clientFd;
pthread_mutex_t lock;
pthread_t thread;
int exitFlag = 1;

int main(int argc, char **argv)
{
    initSignals();
    initServer();
    initThread();
    pid_t pid;

    while (TRUE)
    {
        printf("Waiting for connections...\n");
        if ((clientFd = accept(serverFd, NULL, NULL)) < 0)
        {
            perror("Accept failure");
            exit(1);
        }

        GPSData Data;
        blocking(GET, &Data);

        pid = fork();
        if (!pid)
        {
            serveClient(clientFd);
        }
    }

    close(serverFd);
    printf("Server closed \n");

    return 0;
}