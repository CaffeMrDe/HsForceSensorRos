#include "onRobotEctForceSensoImpl.hpp"
#include "rosconsole/macros_generated.h"
#include <soem/ethercattype.h>
#include <soem/nicdrv.h>
#include <soem/ethercatbase.h>
#include <soem/ethercatmain.h>
#include <soem/ethercatdc.h>
#include <soem/ethercatcoe.h>
#include <soem/ethercatfoe.h>
#include <soem/ethercatconfig.h>
#include <soem/ethercatprint.h>
#include "ros/ros.h"
int onRobotEct::init()
{
//#ifdef ETHERCAT_FUNCTION
    int ret;
    char IOmap[4096];
    int chk;

    // 初始化soem
    ret = ec_init(IFRAME);
    if(ret < 0){
        ROS_INFO_STREAM("ec_init error, ret = " << ret );
        return -1;
    }

    // 初始化并配置 是否自动配置
    ret = ec_config_init(FALSE);
    if(ret < 0){
        ROS_INFO_STREAM("ec_config_init error, ret = " << ret );
        return -1;
    }

    //已搜索到的从站数量
    ROS_INFO_STREAM("ec_slavecount = " << ec_slavecount );
    if(ec_slavecount > 0)
        ROS_INFO_STREAM("ec_slave0 name was: " << ec_slave[1].name );

    ec_config_map(&IOmap);
    ec_configdc();


    ec_statecheck(0, EC_STATE_SAFE_OP,  EC_TIMEOUTSTATE);	// 等待所有从站切换至safe OP状态
    ROS_INFO_STREAM("all slave state was safe op" );

    ec_slave[0].state = EC_STATE_OPERATIONAL;
    ec_send_processdata();
    ec_receive_processdata(EC_TIMEOUTRET);
    ec_writestate(0);

    //等待的状态 计数
    chk = 40;
    do
    {
        ec_send_processdata();
        ec_receive_processdata(EC_TIMEOUTRET);
        ec_statecheck(0, EC_STATE_OPERATIONAL, 50000);
    }
    while (chk-- && (ec_slave[0].state != EC_STATE_OPERATIONAL)); // 等待所有从站切换至OP状态

    ROS_INFO_STREAM("all slave state was op " );

    ROS_INFO_STREAM("---------------------- " << chk);
    ROS_INFO_STREAM("Obits" << ec_slave[1].Ibits );
//#endif
}

int onRobotEct::getForceVal(vector<double> &data)
{
    ec_send_processdata();
    int ret = ec_receive_processdata(EC_TIMEOUTRET);

    buffer = (float *)ec_slave[1].inputs;
    data.clear();
    data.resize(6);
    copy(&buffer[0],&buffer[0] +6, data.begin());
    return ret;
}

std::__cxx11::string onRobotEct::getSensorInfo()
{
    return sensoName;
}
