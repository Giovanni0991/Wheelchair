cmake_minimum_required(VERSION 2.8.3)
project(read_pos_ag)

## Find catkin and any catkin packages
find_package(catkin REQUIRED COMPONENTS gazebo_ros roscpp rospy std_msgs genmsg)
find_package(gazebo REQUIRED)

## Declare ROS messages and services
add_message_files(FILES Num.msg)

## Generate added messages and services
generate_messages(DEPENDENCIES std_msgs)

## Declare a catkin package
catkin_package()

## Build talker and listener
include_directories(
	${catkin_INCLUDE_DIRS}
	${GAZEBO_INCLUDE_DIRS})

link_directories(${GAZEBO_LIBRARY_DIRS})
## Permette di compilare i file in c++ e includere le librerie .hh
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${GAZEBO_CXX_FLAGS}")

add_executable(read_pos_ag src/read_pos_ag.cpp)
add_executable(read_imu src/read_imu.cpp)

target_link_libraries(read_pos_ag ${catkin_LIBRARIES})
target_link_libraries(read_imu ${catkin_LIBRARIES} ${GAZEBO_LIBRARIES})
