// serial_communicator.h
#ifndef SERIAL_COMMUNICATOR_H
#define SERIAL_COMMUNICATOR_H

#include <stdio.h>
#include <windows.h>

extern HANDLE comHandle;

int Useserial(void);
HANDLE OpenSerial(const char *com, int baud, int byteSize, int parity, int stopBits);
void CloseSerial(HANDLE comHandle);
void SendMessageToSerial(const char* message);


#endif /* Serial Communicator */