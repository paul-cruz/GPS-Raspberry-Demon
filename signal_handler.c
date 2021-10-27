#include "thread.h"
#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/wait.h>
#include <sys/types.h>

extern int serverFd, clientFd;
extern pthread_t thread;
extern pthread_mutex_t lock;
extern int exitFlag;

void ISRsw(int signal)
{
	int estado;
	pid_t pid;
	if (signal == SIGCHLD)
	{
		printf("SIGCHLD received\n");
		pid = wait(&estado);
		printf("Proccess with pid %d ended with %d state\n", pid, estado >> 8);
	}
	else if (signal == SIGINT)
	{
		printf("Server closed because of SIGINT signal\n");
		close(clientFd);
		close(serverFd);
		exitFlag = 0;
		pthread_join(thread, NULL);
		pthread_mutex_destroy(&lock);
		exit(0);
	}
}