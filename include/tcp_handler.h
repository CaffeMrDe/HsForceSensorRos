#include <arpa/inet.h>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#pragma pack(1)

#define SAMPLE_COUNT	100		/* 100 incoming samples */

#define READFT				0
#define READCALIBRATIONINFO 1

typedef int SOCKET_HANDLE;
typedef unsigned int uint32;
typedef int int32;
typedef unsigned short uint16;
typedef short int16;
typedef unsigned char byte;

typedef struct FTResponseStruct {
	uint16 header;
	uint16 status;
	int16 ForceX;
	int16 ForceY;
	int16 ForceZ;
	int16 TorqueX;
	int16 TorqueY;
	int16 TorqueZ;
} FTResponse;


typedef struct CalibrationResponseStruct
{
	uint16 header;
	byte forceUnits;
	byte torqueUnits;
	uint32 countsPerForce;
	uint32 countsPerTorque;
	uint16 scaleFactors[6];
} CalibrationResponse;


typedef struct FTReadCommandStruct
{
	byte command;
	byte reserved[19];
}FTReadCommand;


typedef struct ReadCalibrationCommandStruct {
	byte command;
	byte reserved[19];
} ReadCalibrationCommand;

