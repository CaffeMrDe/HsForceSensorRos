#pragma once
#include "ForceSensoInterface.hpp"
#include "tcp_handler.h"

#define PORT			49151	/* Port the Ethernet DAQ always uses */
#define IPPAR "192.168.1.1"

class onRobotEnt : public ForceSensoInterface
{
public:
    explicit onRobotEnt(const string & name):sensoName(name) {}
    ~onRobotEnt();
    virtual int init();
    virtual int getForceVal(vector<double> &data);
    virtual string getSensorInfo();
 private:
    void MySleep(unsigned long ms);
    int Connect(SOCKET_HANDLE * handle, const char * ipAddress, uint16 port);
    void Close(SOCKET_HANDLE * handle);
    void ShowCalibrationInfo(CalibrationResponse * r);
    int GetCalibrationInfo(SOCKET_HANDLE *socket);
    int16 swap_int16(int16 val);
     void SwapFTResponseBytes(FTResponse * r);

     int ReadFT(SOCKET_HANDLE * socket, FTResponse * r);
private:
    CalibrationResponse calibrationResponse;
    FTResponse ftResponse;
    const string sensoName;
    SOCKET_HANDLE socketHandle;
};
