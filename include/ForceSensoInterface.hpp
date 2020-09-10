#pragma once
#include "vector"
#include "iostream"
#include "string"
using namespace std ;

class ForceSensoInterface
{
public:
    virtual int  init() = 0;
    virtual int getForceVal(vector<double> &data) = 0;
    virtual string getSensorInfo() = 0;
};
