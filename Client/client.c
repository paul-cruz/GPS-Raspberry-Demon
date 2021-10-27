#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define PUERTO 8080
#define TAM_BUFFER 128
#define DIR_IP "127.0.0.1"

struct GPSData
{
    char lat[20], lng[20], time[20];
    char verticalCardPoint, horizontalCardPoint;
};

int main(int argc, char **argv)
{
    struct GPSData data;

    int sockFd;
    struct sockaddr_in severAddress;

    memset(&severAddress, 0, sizeof(severAddress));
    severAddress.sin_family = AF_INET;
    severAddress.sin_port = htons(PUERTO);

    if (inet_pton(AF_INET, DIR_IP, &severAddress.sin_addr) <= 0)
    {
        perror("Failed to assign IP address");
        exit(1);
    }

    if ((sockFd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        perror("Failed to create the socket");
        exit(1);
    }

    printf("Connecting to server...\n");
    if (connect(sockFd, (struct sockaddr *)&severAddress, sizeof(severAddress)) < 0)
    {
        perror("Failed to connect with the server");
        exit(1);
    }

    if (read(sockFd, &data, sizeof(data)) < 0)
    {
        perror("Failed to recive data from server");
        exit(1);
    }

    printf("GPS DATA: \nTime: %s\nLat: %s %c\nLng: %s %c\n",
           data.time, data.lat, data.verticalCardPoint, data.lng, data.horizontalCardPoint);

    close(sockFd);
    printf("Client closed\n");
    return 0;
}