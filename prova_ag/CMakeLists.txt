cmake_minimum_required(VERSION 2.8.3)
project(prova_ag)

find_package(catkin REQUIRED COMPONENTS
  roscpp
)


catkin_package(
#  INCLUDE_DIRS include
#  LIBRARIES prova_ag
#  CATKIN_DEPENDS roscpp
#  DEPENDS system_lib
)


include_directories(
	include
	${catkin_INCLUDE_DIRS}
	#${Boost_INCLUDE_DIRS}
)

add_executable(prova_imu src/prova_imu.cpp)

target_link_libraries(prova_imu ${catkin_LIBRARIES})
