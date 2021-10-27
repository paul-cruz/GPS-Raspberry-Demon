#ifndef THREAD_H
#define THREAD_H
#include "defs.h"

void startUartThread();
void *uartReading(void *args);
void blocking(int opcode, GPSData *newData);

#endif
