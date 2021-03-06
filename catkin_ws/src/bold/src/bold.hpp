#ifndef BOLD_HPP
#define BOLD_HPP


#include <ros/ros.h>
#include <image_transport/image_transport.h>

#include <sensor_msgs/image_encodings.h>
#include <cv_bridge/cv_bridge.h>
#include <opencv/cv.h>
#include <opencv2/nonfree/nonfree.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include <math.h>

#include <vector>
#include <iostream>
#include <string>
#include <cfloat>
#include <ctime>

#include "bold_vector.hpp"
#include "bold_feature.hpp"
#include "bold_datum.hpp"


using namespace cv;
using namespace std;

namespace BOLD{
  
  enum BOLDConstants{
    
    K_NEAREST_LINE_SEGMENTS = 10, 
  };
  

  
  

  
  struct Line{
  
    double x1,y1,x2,y2,width,p,minLogNFA;
  };
  
  
  
  class BOLDescriptor{
  private: 
    
    std::vector<int> falseLines;
    double* image;
    int imWidth,imHeight;
    Line* lines;
    bool imageIsSet;
    bool linesIsSet;
    int nLines;
    BOLDFeature* feature;
    int KNLIndices[K_NEAREST_LINE_SEGMENTS];
    
    void kNearestLines(int index);
    void detectLines();
    void resolveAngles(int indexA,int indexB);
    BVector getGradient(int x,int y);
    double getImValue(int x,int y);
    string imageName;
    
    
  public:
    BOLDescriptor();
    ~BOLDescriptor();
    void setImage(Mat image,bool showImage);
    void setImage(string fileName,bool showImage);
    void describe();
    BOLDFeature* getFeature();
    void showLines();
    void showFeatures();
    void setImageName(string name);
    void setFeatureLabel(BOLDDatum dat);
    string getImageName();
    void clear();
    void freeFeature();
  };
}
#endif 