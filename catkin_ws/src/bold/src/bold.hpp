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

#include <math.h>

#include <vector>
#include <iostream>
#include <string>


#include "bold_vector.hpp"



using namespace cv;
using namespace std;

namespace BOLD{
  
  enum BOLDConstants{
    HISTOGRAM_SIZE = 10,
    
  };
  

  
  
  class BOLDFeature{
  private:
   int histogram[HISTOGRAM_SIZE][HISTOGRAM_SIZE];
  public:
   BOLDFeature();
  };
  
  class Line{
  public:
    double x1,y1,x2,y2,width,p,minLogNFA;
    //double x1,y1,x2,y2,gradient;
  };
  
  
  
  class BOLDescriptor{
  private: 
    void detectLines();
    double* image;
    int imWidth,imHeight;
    Line* lines;
    bool imageIsSet;
    int nLines;
    BOLDFeature feature;
    double histBinSize;
    BVector getGradient(int x,int y);
    double getImValue(int x,int y);
  public:
    BOLDescriptor();
    ~BOLDescriptor();
    void setImage(Mat image);
    void describe();
    BOLDFeature getFeature();
  
  };
}
#endif 