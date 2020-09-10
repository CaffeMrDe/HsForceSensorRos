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
#include <LantEctForceSensoImpl.h>
#include <boost/thread.hpp>
#define IFRAME "enp4s0"



LanEctForceSensorImpl::LanEctForceSensorImpl(const string &name)
{
    sensoName = name;
}

int LanEctForceSensorImpl::init()
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

    //配置 ethercat 的 IOmap大小
    ec_config_map(&IOmap);
    ec_configdc();


    //状态检查
    ec_statecheck(0, EC_STATE_SAFE_OP,  EC_TIMEOUTSTATE);	// 等待所有从站切换至safe OP状态
    ROS_INFO_STREAM("all slave state was safe op" );

    //设置状态
    ec_slave[0].state = EC_STATE_OPERATIONAL;
    ec_send_processdata();
    ec_receive_processdata(EC_TIMEOUTRET);
    ec_writestate(0);

    //等待的状态 计数
    chk = 40;
    ROS_INFO_STREAM("Set all slave state to EC_STATE_OPERATIONAL " );

    do
    {
        ec_send_processdata();
        ec_receive_processdata(EC_TIMEOUTRET);
        ec_statecheck(0, EC_STATE_OPERATIONAL, 50000);
        if(chk == 1)
        {
            return -1;
        }
    }
    while (chk-- && (ec_slave[0].state != EC_STATE_OPERATIONAL)); // 等待所有从站切换至OP状态

    ROS_INFO_STREAM("Set all slave state to EC_STATE_OPERATIONAL " );

    ROS_INFO_STREAM("---------------------- " << chk);
    ROS_INFO_STREAM("Obits : " << ec_slave[1].Ibits );

    boost::this_thread::sleep_for(boost::chrono::milliseconds(500));
    return 0;
}

int LanEctForceSensorImpl::getForceVal(vector<double> &data)
{
    //
    ec_send_processdata();
    int ret = ec_receive_processdata(EC_TIMEOUTRET);

    if(ret != 1)
        return -1;

    //
    buffer = (int *)ec_slave[1].inputs;
    array<int, 9> temp;
    copy(&buffer[0], &buffer[0] + temp.size(), temp.begin());

    //
    data.clear();
    data.resize(6);
    for(int i = 0; i < 6; i++){
        data[i] = (static_cast<double>(temp[i])/100000)*10; //N 为单位
    }

    //第7位为 传感器的状态
    ret = temp[6];
//    for(int i = 0; i <temp.size(); i++)
//        std::cout << temp[i]<<" ";
//    std::cout <<std::endl;

    return ret;
}

std::__cxx11::string LanEctForceSensorImpl::getSensorInfo()
{
    return sensoName;
}
