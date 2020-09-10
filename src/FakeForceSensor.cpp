#include <FakeForceSensor.h>

////当前的采集周期是i 10 / 5000
double FakeForceSenso::timeStamp = 10;
double FakeForceSenso::fHz = 1000;

double FakeForceSenso::piValue = M_PI;
double FakeForceSenso::gain = 3;
FakeForceSenso::FakeForceSenso(const string &path):name(path)
{

}

int FakeForceSenso::init()
{
    sensorEnable = {false,false,false,true,true,true};

    timeStamp = 10; // 1 /1000 分数
    startTime = timeStamp;
    fakeTime = 0;
    return 0;
}

int FakeForceSenso::getForceVal(vector<double> &data)
{
    //
    data.clear();
    data.resize(6);
    std::fill(data.begin(), data.begin() + data.size(), 0);

    //获取基准数值
    static double tempValue = 0;
    tempValue = startTime/ fHz;

    //遍历数组
    for(int i = 0; i< sensorEnable.size(); i++)
    {
        if(!sensorEnable[i])
            continue;
        data[i] = drawSin(tempValue);
    }

    startTime += timeStamp ;

    if(startTime > fHz)
    {
        startTime =0;
    }

    return 0;

}

string FakeForceSenso::getSensorInfo()
{
    return name;
}

double FakeForceSenso::drawSin(double& x)
{
    return gain *sin(x*piValue);
}

