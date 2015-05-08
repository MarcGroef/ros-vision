#ifndef BOLD_HPP
#define BOLD_HPP



#include <ros/ros.h>
#include <image_transport/image_transport.h>
#include <cv_bridge/cv_bridge.h>
#include <sensor_msgs/image_encodings.h>

#include <opencv/cv.h>
#include <opencv2/nonfree/nonfree.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include <vector>
#include <iostream>
#include <string>




using namespace cv;
using namespace std;

namespace BOLD{
  
  
  class Line{
    double x1,y1,x2,y2,width,p,minLogNFA;
  };
  
  
  
  class BOLDescriptor{

    
  public:
    BOLDescriptor();
    void setImage(Mat image);
    void describe();
  private: 
    void detectLines();
    cv::Mat image;
    Line* lines;
    bool imageIsSet;
    int nLines;
  };
}
#endif 