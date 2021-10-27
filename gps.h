#ifndef GPS_H
#define GPS_H

int validateGPGGALine(char *line);
void readGPS(int serial, GPSData *data);
int analizeLine(char *line, GPSData *data);
int configSerial(char *deviceFd, speed_t baud);
void readSerialLine(char line[128], int serial);
void copyGPSData(GPSData *origin, GPSData *dest);

#endif
