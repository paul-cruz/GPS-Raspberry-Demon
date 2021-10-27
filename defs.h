#ifndef DEFS_H
#define DEFS_H

#define PORT 8080
#define CLIENTS_QUEUE 5
#define BUFFER_SIZE 128
#define TRUE 1
#define FALSE 0
#define GET 1
#define SET 2
#define N 10

typedef struct
{
    char lat[20], lng[20], time[20];
    char verticalCardPoint, horizontalCardPoint;
} GPSData;

#endif