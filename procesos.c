#include "defs.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

extern GPSData gpsData;

void serveClient(int clientFd)
{
	if (write(clientFd, &gpsData, sizeof(gpsData)) < 0)
	{
		perror("Failure at sending data to the client");
		exit(EXIT_FAILURE);
	}
	close(clientFd);
	printf("Client served\n");
	exit(0);
}