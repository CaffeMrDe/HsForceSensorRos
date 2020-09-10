#include <iostream>
#include <unistd.h>
#include <stdio.h>
#include <string.h>

#include <soem/ethercattype.h>
#include <soem/nicdrv.h>
#include <soem/ethercatbase.h>
#include <soem/ethercatmain.h>
#include <soem/ethercatdc.h>
#include <soem/ethercatcoe.h>
#include <soem/ethercatfoe.h>
#include <soem/ethercatconfig.h>
#include <soem/ethercatprint.h>

#include <ros/ros.h>
#include "geometry_msgs/WrenchStamped.h"
#include "hirop_msgs/getOnRobotForce.h"
#include "hirop_msgs/setOnRobotForce.h"
#include <thread>
#include "unistd.h"
#include "csignal"

#include "ForceSensoInterface.hpp"
#include "ForceSensoFactory.hpp"

using namespace std;
bool running = false;
float *buffer;
ros::Publisher state_pub;


//#define ETHERCAT_FUNCTION
//#define ETHERCAT_FUNCTION 1
#define LAND_ETHERCAT_FUNCTION 2
boost::shared_ptr<ros::NodeHandle> n;
std::shared_ptr<ForceSensoInterface> ptr;



void runOnRobotThread()
{
    int ret = -1;
#ifdef ETHERCAT_FUNCTION
    ros::Rate loop_rate(200);
#else
    ros::Rate loop_rate(100);
#endif
    ROS_INFO_STREAM("runOnRobotThread running...");
    geometry_msgs::Wrench tmp;
    vector<double> force;
    while(ros::ok() && running){
       force.clear();
       ptr->getForceVal(force);
       tmp.force.x = force[0]; tmp.force.y = force[1];tmp.force.z = force[2];
       tmp.torque.x = force[3];tmp.torque.y = force[4];tmp.torque.z = force[5];

       state_pub.publish(tmp);
       loop_rate.sleep();
    }
    ROS_INFO_STREAM("runOnRobotThread exit...");

}

void signalHandler(int signum)
{
  ROS_INFO("%s is received, Terminating the node...",strsignal(signum));
//  n->shutdown();
  exit(signum);
}

bool OnRobotEthercatServer(hirop_msgs::setOnRobotForceRequest& req,hirop_msgs::setOnRobotForceResponse& res){
    if(req.enable && running == false)
    {
        running = true;
        std::thread t1(runOnRobotThread);
        t1.detach();

    }else if(running == true && (!req.enable))
    {
        running = false;
    }
    return true;
}

int main(int argc, char **argv){
    ros::init(argc, argv, "onRobot_daq_driver");
    ros::NodeHandle n;

#ifdef defined(ETHERCAT_FUNCTION)
    ptr = std::shared_ptr<ForceSensoInterface> (forceSensorFactoty::create(ForceSort::onRobotEthercat));
#elif defined(ETHERNET_FUNCTION)
    ptr = std::shared_ptr<ForceSensoInterface> (forceSensorFactoty::create(ForceSort::onRobotEthernet));
#elif defined(FAKE_FUNCTION)
    ptr = std::shared_ptr<ForceSensoInterface> (forceSensorFactoty::create(ForceSort::MockSensor));
#elif LAND_ETHERCAT_FUNCTION
    ptr = std::shared_ptr<ForceSensoInterface> (forceSensorFactoty::create(ForceSort::LandDianEthrecat));

#endif
    assert(ptr->init() == 0);
    ROS_INFO_STREAM( "getSensorInfo : "<<ptr->getSensorInfo());

    state_pub = n.advertise<geometry_msgs::Wrench>("daq_data", 1000);

    ros::ServiceServer service = n.advertiseService("set_onRobot_force_enable",OnRobotEthercatServer);
    ROS_INFO_STREAM( "onRobot_daq_driver start ....");
    signal(SIGINT,signalHandler);

    running = true;
    std::thread t1(runOnRobotThread);
    t1.detach();
    ros::spin();

	return 0;
}



