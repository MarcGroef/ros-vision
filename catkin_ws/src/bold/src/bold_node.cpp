#include <iostream>
#include <ros/ros.h>
#include <std_msgs/String.h>

using namespace ros;

int main(int argc,char**argv){
  init(argc,argv, "bold");
  NodeHandle n;
  Publisher publ = n.advertise<std_msgs::String>("bold",10);
  Rate loop_rate(10);
  
  for(int count = 0;ok();count++){
    std_msgs::String  msg;
    std::stringstream ss;
    ss << "Hello, world!" << count;
    msg.data = ss.str();
    
    ROS_INFO("%s",msg.data.c_str());
    
    publ.publish(msg);
    spinOnce();
    loop_rate.sleep();
    
    
  }
  
  
  return 0;
}