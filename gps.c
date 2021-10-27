#include <stdio.h>
#include <stdlib.h>
#include <termios.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include "defs.h"

int configSerial(char *deviceFd, speed_t baud)
{
    struct termios newtermios;
    int fd;

    fd = open(deviceFd, (O_RDWR | O_NOCTTY) & ~O_NONBLOCK);

    if (fd == -1)
    {
        printf("Failure at trying to open %s\n", deviceFd);
        exit(EXIT_FAILURE);
    }

    newtermios.c_cflag = CBAUD | CS8 | CLOCAL | CREAD;
    newtermios.c_iflag = IGNPAR;
    newtermios.c_oflag = 0;
    newtermios.c_lflag = TCIOFLUSH | ~ICANON;
    newtermios.c_cc[VMIN] = 1;
    newtermios.c_cc[VTIME] = 0;

    if (cfsetospeed(&newtermios, baud) == -1)
    {
        printf("Failure at setting output baud\n");
        exit(EXIT_FAILURE);
    }

    if (cfsetispeed(&newtermios, baud) == -1)
    {
        printf("Failure at setting input baud\n");
        exit(EXIT_FAILURE);
    }

    if (tcflush(fd, TCIFLUSH) == -1)
    {
        printf("Failure at TCIFLUSH\n");
        exit(EXIT_FAILURE);
    }

    if (tcflush(fd, TCOFLUSH) == -1)
    {
        printf("Failure at TCOFLUSH\n");
        exit(EXIT_FAILURE);
    }

    if (tcsetattr(fd, TCSANOW, &newtermios) == -1)
    {
        printf("Failure at setting terminal params\n");
        exit(EXIT_FAILURE);
    }

    return fd;
}

void readSerialLine(char line[128], int serial)
{
    register int i = 0;
    do
    {
        read(serial, &line[i], 1);
    } while (line[i++] != '\n');

    line[i] = 0;
}

int validateGPGGALine(char *line)
{
    register int i, j = 0;
    for (i = 7; i < strlen(line); i++)
    {
        if (line[i] == ',')
        {
            return FALSE;
        }
        while (line[i] != ',')
        {
            i++;
        }
        if (++j == 5)
        {
            return TRUE;
        }
    }
    return FALSE;
}

int analizeLine(char *line, GPSData *data)
{
    register int i;
    char nmeaParams[5][BUFFER_SIZE];
    char auxLine[BUFFER_SIZE];
    strcpy(auxLine, line);

    if (strcmp(strtok(line, ","), "$GPGGA"))
    {
        return FALSE;
    }

    if (validateGPGGALine(auxLine) == FALSE)
    {
        printf("Invalid GPGGA line: %s\n", auxLine);
        return FALSE;
    }

    for (i = 0; i < 5; ++i)
    {
        strcpy(nmeaParams[i], strtok(NULL, ","));
    }

    stpcpy(data->time, nmeaParams[0]);
    stpcpy(data->lat, nmeaParams[1]);
    data->verticalCardPoint = nmeaParams[2][0];
    stpcpy(data->lng, nmeaParams[3]);
    data->horizontalCardPoint = nmeaParams[4][0];
    printf("GPS Data updated\n");
    return TRUE;
}

void readGPS(int serial, GPSData *data)
{
    char line[BUFFER_SIZE];
    memset(line, '\0', BUFFER_SIZE);
    do
    {
        readSerialLine(line, serial);
    } while (analizeLine(line, data) == FALSE);
}

void copyGPSData(GPSData *origin, GPSData *dest)
{
    strcpy(dest->time, origin->time);
    strcpy(dest->lat, origin->lat);
    dest->verticalCardPoint = origin->verticalCardPoint;
    strcpy(dest->lng, origin->lng);
    dest->horizontalCardPoint = origin->horizontalCardPoint;
}