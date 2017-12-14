#include <ros/ros.h>
#include <ros/package.h>
#include <string>
#include <math.h>
#include <sstream>

#include <geometry_msgs/PoseStamped.h>
#include <geometry_msgs/Point.h>
#include <nav_msgs/Odometry.h>
#include <yaml-cpp/yaml.h>
using namespace std;

// global variables for subscriber
bool odom_ok;
geometry_msgs::Pose pose;

void poseCallback(const nav_msgs::OdometryConstPtr & msg)
{
    pose = msg->pose.pose;
    odom_ok = true;
}

// global var for WP reader
YAML::Node wp;
double WPcoord(int idx, std::string var)
{
    return wp[idx][var].as<double>();
}

int main (int argc, char** argv)
{
    ros::init(argc, argv, "waypoint");
    ros::NodeHandle nh;

    // subscriber
    odom_ok = false;
    ros::Subscriber state_sub = nh.subscribe<nav_msgs::Odometry> ("/auv/state", 1, poseCallback);

    // publisher
    ros::Publisher setpoint_pub = nh.advertise<geometry_msgs::PoseStamped>("/auv/body_position_setpoint", 1);
    geometry_msgs::PoseStamped setpoint;
    setpoint.header.frame_id = "world";

    // load waypoints
    std::string wp_path = ros::package::getPath("ecn_rasom") + "/config/waypoints.yaml";
    YAML::Node node = YAML::LoadFile(wp_path);
    // get thresholds
    const double thr = node["threshold"].as<double>();
    std::cout << "WP Threshold: " << thr << std::endl;
    const double thr_angle = node["threshold_angle"].as<double>();
    std::cout << "WP Angle Threshold: " << thr_angle << std::endl;

    // get waypoints
    wp = node["wp"];
    std::cout << "Found " << wp.size() << " waypoints" << std::endl;
    int idx;
    for(idx = 0; idx < wp.size(); ++idx)
    {
        std::cout << "   wp #" << idx << ": ";
        std::cout << "x = " << WPcoord(idx, "x");
        std::cout << ", y = " << WPcoord(idx, "y");
        std::cout << ", z = " << WPcoord(idx, "z");
        std::cout << ", theta = " << WPcoord(idx, "theta");
        std::cout << std::endl;
    }

    // write first WP
    setpoint.pose.position.x = WPcoord(0, "x");
    setpoint.pose.position.y = WPcoord(0, "y");
    setpoint.pose.position.z = WPcoord(0, "z");
    setpoint.pose.orientation.x = 0;
    setpoint.pose.orientation.y = 0;
    setpoint.pose.orientation.z = sin(WPcoord(0, "theta")/2);
    setpoint.pose.orientation.w = cos(WPcoord(0, "theta")/2);

    ros::Rate rate(10);

    // waypoint index
    idx = 0;

    while (ros::ok())
    {
        if(odom_ok)
        {
            // check Cartesian distance to current wp
            // update setpoint if needed




            // publish setpoint
            setpoint.header.stamp = ros::Time::now();
            setpoint_pub.publish(setpoint);
        }
        ros::spinOnce();
        rate.sleep();
    }
}
