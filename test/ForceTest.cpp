#include <gtest/gtest.h>
#include <FakeForceSensor.h>
#include <boost/shared_ptr.hpp>
#include <boost/make_shared.hpp>

#include "ForceSensoInterface.hpp"
#include "ForceSensoFactory.hpp"

class MYTestClass : public testing::Test {
protected:
    virtual void SetUp() {
        // some initialization of testing
//        ptr = boost::make_shared<FakeForceSenso>("fake");
        ptr = boost::shared_ptr<ForceSensoInterface>(forceSensorFactoty::create(ForceSort::LandDianEthrecat));
    }
    virtual void TearDown() {

    }
protected:
    boost::shared_ptr<ForceSensoInterface> ptr;
    // some member variance
};

TEST_F(MYTestClass, my_test_1) {
   // some test operation
    ptr->init();
    vector<double> data;

    for(;;)
    {
        int ret = ptr->getForceVal(data);
        std::cout << "ret : "<<ret<<std::endl;
        ASSERT_EQ(ret, 0);
        for(auto it : data)
            std::cout << it <<" ";
        std::cout << std::endl;
    }
}




int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);// gtest 的初始化

   return RUN_ALL_TESTS();//调用 GTest 测试用例
}
