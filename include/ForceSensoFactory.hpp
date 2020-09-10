#pragma once

#include "ForceSensoInterface.hpp"
enum ForceSort{
    onRobotEthercat,
    onRobotEthernet,
    MockSensor,
    LandDianEthrecat,
};


class forceSensorFactoty
{
public:
    static ForceSensoInterface * create(ForceSort sort);
private:
    static ForceSensoInterface* handler;
};
