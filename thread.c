#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <termios.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include "defs.h"
#include "gps.h"

extern GPSData gpsData;
extern pthread_mutex_t lock;
extern pthread_t thread;
extern int exitFlag;

void blocking(int opcode, GPSData *newData)
{
    pthread_mutex_lock(&lock);

    if (opcode == GET)
    {
        copyGPSData(&gpsData, newData);
    }
    else
    {
        copyGPSData(newData, &gpsData);
    }

    pthread_mutex_unlock(&lock);
}

void *uartReading(void *args)
{
    int serial = configSerial("/dev/ttyS0", B9600);

    GPSData newData;
    while (exitFlag)
    {
        printf("exitFlag: %d\n", exitFlag);
        readGPS(serial, &newData);
        blocking(SET, &newData);
    }

    return NULL;
}

void startUartThread()
{
    pthread_create(&thread, NULL, uartReading, NULL);
}