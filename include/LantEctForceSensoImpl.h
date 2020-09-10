#pragma once
#include <soem/ethercattype.h>
#include <soem/nicdrv.h>
#include <soem/ethercatbase.h>
#include <soem/ethercatmain.h>
#include <soem/ethercatdc.h>
#include <soem/ethercatcoe.h>
#include <soem/ethercatfoe.h>
#include <soem/ethercatconfig.h>
#include <soem/ethercatprint.h>
//#include "ros/ros.h"
#include <ForceSensoInterface.hpp>
class LanEctForceSensorImpl:  public ForceSensoInterface
{
public:
    explicit LanEctForceSensorImpl(const string & name);
    virtual int  init();
    virtual int getForceVal(vector<double> &data);
    virtual string getSensorInfo();
private:
    string sensoName;
    int *buffer;
};
