#include "ForceSensoFactory.hpp"
#include "onRobotEctForceSensoImpl.hpp"
#include "onRobotEntForceSensoImpl.hpp"
#include <LantEctForceSensoImpl.h>
#include <FakeForceSensor.h>
ForceSensoInterface* forceSensorFactoty::handler = nullptr;

ForceSensoInterface *forceSensorFactoty::create(ForceSort sort)
{

    if(handler != nullptr)
        return nullptr;
    switch (sort) {
    case onRobotEthernet:
        handler = new onRobotEnt("onRobot Ethernet");
        break;
     case onRobotEthercat:
        handler = new onRobotEct("onRobot Ethercat");
        break;
    case MockSensor:
       handler = new FakeForceSenso("onRobot Fake");
       break;
    case LandDianEthrecat:
        handler = new LanEctForceSensorImpl(string("LandDianEthrecat Ethercat"));
        break;
    default:
        perror("bad....");
        break;
    }
    return handler;
}
