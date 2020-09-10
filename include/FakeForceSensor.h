#pragma once
#include "vector"
#include "iostream"
#include "string"
#include <math.h>
#include <array>
using namespace std ;
#include <ForceSensoInterface.hpp>
class FakeForceSenso :public ForceSensoInterface
{
public:
    explicit FakeForceSenso(const string & path= "Fake");
    virtual int  init();
    virtual int getForceVal(vector<double> &data);
    virtual string getSensorInfo();
private:
    double drawSin(double &x);
private:
    int fakeTime;
    double startTime;
    string name;
    static double timeStamp;
    static double fHz;
    static double piValue;
    static double gain;

    std::array<bool, 6> sensorEnable;

};


