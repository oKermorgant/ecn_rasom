cmake_minimum_required(VERSION 2.8.3)
project(ecn_rasom)

## Find catkin macros and libraries
## if COMPONENTS list like find_package(catkin REQUIRED COMPONENTS xyz)
## is used, also find other catkin packages
find_package(catkin REQUIRED COMPONENTS
  nav_msgs
  roscpp
  sensor_msgs
  freefloating_gazebo
)


include_directories(
  ${catkin_INCLUDE_DIRS}
)

## Declare a C++ executable
# add_executable(ecn_rasom_node ecn_rasom_node.cpp)


## Specify libraries to link a library or executable target against
# target_link_libraries(ecn_rasom_node
#   ${catkin_LIBRARIES}
# )

# specify dependency on custom messages
#add_dependencies(ecn_rasom_node freefloating_gazebo_generate_messages_cpp)
