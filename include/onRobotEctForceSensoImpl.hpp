#pragma once
#include "ForceSensoInterface.hpp"
#define IFRAME "enp4s0"

class onRobotEct : public ForceSensoInterface
{
public:
    explicit onRobotEct(const string & name):sensoName(name) {}
    virtual int init();
    virtual int getForceVal(vector<double> &data);
    virtual string getSensorInfo();
private:
    const string sensoName;
    float *buffer;
};
