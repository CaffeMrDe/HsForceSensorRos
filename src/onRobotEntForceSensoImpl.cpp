#include "onRobotEntForceSensoImpl.hpp"
onRobotEnt::~onRobotEnt()
{
    if(socketHandle >= 0)
        Close(&socketHandle);
}

int onRobotEnt::init()
{

    if (Connect(&socketHandle, IPPAR, PORT) != 0) {
        fprintf(stderr, "Could not connect to device...\r\n");
        return -1;
    }
    else {
        fprintf(stdout, "Connected to Ethernet DAQ\r\n");
        fflush(stdout);
    }
    if (GetCalibrationInfo(&socketHandle) != 0) {
        fprintf(stderr, "Could not read calibration info...\r\n");
        return -1;
    }
    ShowCalibrationInfo(&calibrationResponse);

    MySleep(2000);  // To allow user to read calibration information sent by Ethernet DAQ
}

int onRobotEnt::getForceVal(vector<double> &data)
{
    data.clear();
    data.resize(6);
    int readSuccess = ReadFT(&socketHandle, &ftResponse);
    if (readSuccess == 0) {
        data[0] = (double)ftResponse.ForceX / (double)calibrationResponse.countsPerForce * (double)calibrationResponse.scaleFactors[0];
        data[1] = (double)ftResponse.ForceY / (double)calibrationResponse.countsPerForce * (double)calibrationResponse.scaleFactors[1];
        data[2] = (double)ftResponse.ForceZ / (double)calibrationResponse.countsPerForce * (double)calibrationResponse.scaleFactors[2];
        data[3] = (double)ftResponse.TorqueX / (double)calibrationResponse.countsPerTorque * (double)calibrationResponse.scaleFactors[3];
        data[4] = (double)ftResponse.TorqueY / (double)calibrationResponse.countsPerTorque * (double)calibrationResponse.scaleFactors[4];
        data[5] = (double)ftResponse.TorqueZ / (double)calibrationResponse.countsPerTorque * (double)calibrationResponse.scaleFactors[5];
    }
    else {
        fprintf(stderr, "Could not read F/T data, error code: %d\r\n", readSuccess);
    }
    return readSuccess;
}

std::__cxx11::string onRobotEnt::getSensorInfo()
{
    std::cout << "sensoName : "<<sensoName<<std::endl;
    return sensoName;
}

/* Sleep ms milliseconds */
void onRobotEnt::MySleep(unsigned long ms)
{
#ifdef _WIN32
    Sleep(ms);
#else
    usleep(ms * 1000);
#endif
}


int onRobotEnt::Connect(SOCKET_HANDLE * handle, const char * ipAddress, uint16 port)
{
    struct sockaddr_in addr;
    struct hostent *he;
    int err;
#ifdef _WIN32
    WSADATA wsaData;
    WORD wVersionRequested;
    wVersionRequested = MAKEWORD(2, 2);
    WSAStartup(wVersionRequested, &wsaData);
    if (GetLastError() != 0) {
        return -1;
    }
#endif

    *handle = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
#ifdef _WIN32
    if (*handle == INVALID_SOCKET) {
#else
    if (*handle == -1) {
#endif
        fprintf(stderr, "Socket could not be opened.\n");
        return -2;
    }
    he = gethostbyname(ipAddress);
    memcpy(&addr.sin_addr, he->h_addr_list[0], he->h_length);
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    fprintf(stdout, "Connecting to EtherDAQ\r\n");
    fflush(stdout);
    err = connect(*handle, (struct sockaddr *)&addr, sizeof(addr));
    if (err < 0) {
        return -3;
    }
    return 0;
}

void onRobotEnt::Close(SOCKET_HANDLE * handle)
{
#ifdef _WIN32
    closesocket(*handle);
    WSACleanup();
#else
    close(*handle);
#endif
}


void onRobotEnt::ShowCalibrationInfo(CalibrationResponse * r)
{
    int i;
    if (r == NULL) {
        return;
    }
    fprintf(stdout, "Header: 0x%x\r\nForcfe Units: %u\r\nTorque Units: %u\r\nCounts Per Force: %u\r\nCounts Per Torque: %u\r\n",
        r->header, r->forceUnits, r->torqueUnits, r->countsPerForce, r->countsPerTorque);


    for (i = 0; i < 6; ++i) {
        fprintf(stdout, "scaleFactors[%d]: %u\r\n", i, r->scaleFactors[i]);
    }
    fflush(stdout);
}

int onRobotEnt::GetCalibrationInfo(SOCKET_HANDLE *socket)
{
    int i;
    int sendSuccess;
    int readSuccess;
    ReadCalibrationCommand readCommand = { 0 };
    readCommand.command = READCALIBRATIONINFO;
    sendSuccess = send(*socket, (const char *)&readCommand, sizeof(ReadCalibrationCommand), 0);
    if (sendSuccess < 0) {
        return sendSuccess;
    }

    readSuccess = recv(*socket, (char *)&calibrationResponse, sizeof(CalibrationResponse), 0);
    if (readSuccess < 0) {
        return readSuccess;
    }
    calibrationResponse.header = htons(calibrationResponse.header);
    calibrationResponse.countsPerForce = ntohl(calibrationResponse.countsPerForce);
    calibrationResponse.countsPerTorque = ntohl(calibrationResponse.countsPerTorque);
    for (i = 0; i < 6; ++i) {
        calibrationResponse.scaleFactors[i] = htons(calibrationResponse.scaleFactors[i]);
    }
    if (calibrationResponse.header != 0x1234) {
        return -1;
    }
    return 0;
}


int16 onRobotEnt::swap_int16(int16 val)
{
    return (val << 8) | ((val >> 8) & 0xFF);
}

 void onRobotEnt::SwapFTResponseBytes(FTResponse * r)
{
    r->header = htons(r->header);
    r->status = htons(r->status);
    r->ForceX = swap_int16(r->ForceX);
    r->ForceY = swap_int16(r->ForceY);
    r->ForceZ = swap_int16(r->ForceZ);
    r->TorqueX = swap_int16(r->TorqueX);
    r->TorqueY = swap_int16(r->TorqueY);
    r->TorqueZ = swap_int16(r->TorqueZ);
}


int onRobotEnt::ReadFT(SOCKET_HANDLE * socket, FTResponse * r)
{
    FTReadCommand readCommand = { 0 };
    int readSuccess;
    int sendSuccess;
    readCommand.command = READFT;
    sendSuccess = send(*socket, (char *)&readCommand, sizeof(FTReadCommand), 0);
    if (sendSuccess < 0) {
        return sendSuccess;
    }
    readSuccess = recv(*socket, (char *)r, sizeof(FTResponse), 0);
    if (readSuccess != sizeof(FTResponse)) {
        return 1;
    }
    SwapFTResponseBytes(r);
    if (r->header != 0x1234) {
        return 2;
    }
    return 0;
}


