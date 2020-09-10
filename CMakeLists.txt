cmake_minimum_required(VERSION 2.8.3)
project(daq_ros_driver)

## Compile as C++11, supported in ROS Kinetic and newer
add_compile_options(-std=c++11)

## Find catkin macros and libraries
## if COMPONENTS list like find_package(catkin REQUIRED COMPONENTS xyz)
## is used, also find other catkin packages
find_package(catkin REQUIRED COMPONENTS
  roscpp
  hirop_msgs
)


find_package(soem REQUIRED)

catkin_package()

## Specify additional locations of header files
## Your package locations should be listed before other locations

include_directories(
 include
  ${catkin_INCLUDE_DIRS}
  ${soem_INCLUDE_DIRS}/soem
  /home/de/catkin_ws/src/daq_ros_driver/include
)

add_executable(daq_ros_driver
    src/driver.cpp
    src/FakeForceSensor.cpp
    src/onRobotEntForceSensoImpl.cpp
    src/onRobotEctForceSensoImpl.cpp
    src/LantEctForceSensoImpl.cpp
    src/ForceSensoFactory.cpp)
#add_dependencies(daq_ros_driver ${hirop_msgs_EXPORTED_TARGETS})

## Specify libraries to link a library or executable target against
target_link_libraries(daq_ros_driver
   ${catkin_LIBRARIES}
   /usr/local/lib/libsoem.a
   pthread
)

set(DGTEST on)
### test
if(DGTEST)
    add_executable(daq_ros_driver_test
        test/ForceTest.cpp
#        src/driver.cpp
        src/FakeForceSensor.cpp
        src/onRobotEntForceSensoImpl.cpp
        src/onRobotEctForceSensoImpl.cpp
        src/LantEctForceSensoImpl.cpp
        src/ForceSensoFactory.cpp)
    target_link_libraries(daq_ros_driver_test
       ${catkin_LIBRARIES}
       gtest
       /usr/local/lib/libsoem.a
       pthread
    )
endif()
