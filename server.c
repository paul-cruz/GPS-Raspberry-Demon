#include "defs.h"
#include "thread.h"
#include "signal_handler.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <pthread.h>

extern int serverFd;
extern pthread_mutex_t lock;

void initServer()
{
	struct sockaddr_in serverAddress;

	memset(&serverAddress, 0, sizeof(serverAddress));
	serverAddress.sin_family = AF_INET;
	serverAddress.sin_port = htons(PORT);
	serverAddress.sin_addr.s_addr = INADDR_ANY;

	if ((serverFd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
	{
		perror("Failed to create the socket");
		exit(EXIT_FAILURE);
	}

	if (bind(serverFd, (struct sockaddr *)&serverAddress, sizeof(serverAddress)) < 0)
	{
		perror("Failed to bind the socket");
		exit(EXIT_FAILURE);
	}

	if (listen(serverFd, CLIENTS_QUEUE) < 0)
	{
		perror("Failed to create the clients queue");
		exit(EXIT_FAILURE);
	}
}

void initThread()
{
	pthread_mutex_init(&lock, NULL);
	startUartThread();
}

void initSignals()
{
	if (signal(SIGCHLD, ISRsw) == SIG_ERR)
	{
		perror("Failure in handle SIGCHLD signal");
		exit(EXIT_FAILURE);
	}
	if (signal(SIGINT, ISRsw) == SIG_ERR)
	{
		perror("Failure in handle SIGINT signal");
		exit(EXIT_FAILURE);
	}
}