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
#include <cfloat>

#include "bold_vector.hpp"



using namespace cv;
using namespace std;

namespace BOLD{
  
  enum BOLDConstants{
    HISTOGRAM_SIZE = 10,
    FEATURE_SHOW_SIZE = 200,
    K_NEAREST_LINE_SEGMENTS = 10, 
  };
  

  
  
  class BOLDFeature{
  private:
   int histogram[HISTOGRAM_SIZE][HISTOGRAM_SIZE];//  [alphas][betas]
   int entries;
   double histBinSize;
   int highestCount;
  public:
   BOLDFeature();
   void add(double alha,double beta);
   void print();
   void show();
   
  };
  
  class Line{
  public:
    double x1,y1,x2,y2,width,p,minLogNFA;
  };
  
  
  
  class BOLDescriptor{
  private: 
    
    std::vector<int> falseLines;
    double* image;
    int imWidth,imHeight;
    Line* lines;
    bool imageIsSet;
    int nLines;
    BOLDFeature feature;
    int KNLIndices[K_NEAREST_LINE_SEGMENTS];
    
    void kNearestLines(int index);
    void detectLines();
    void resolveAngles(int indexA,int indexB);
    BVector getGradient(int x,int y);
    double getImValue(int x,int y);
  public:
    BOLDescriptor();
    ~BOLDescriptor();
    void setImage(Mat image);
    void describe();
    BOLDFeature getFeature();
    void showLines();
    void showFeatures();
  };
}
#endif 