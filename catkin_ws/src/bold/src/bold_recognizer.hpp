#ifndef BOLD_RECOGNIZER_HPP
#define BOLD_RECOGNIZER_HPP

#include "bold.hpp"
#include "bold_feature.hpp"
#include "bold_datum.hpp"


#include <ros/ros.h>
#include <image_transport/image_transport.h>
#include <cv_bridge/cv_bridge.h>
#include <sensor_msgs/image_encodings.h>

#include <opencv/cv.h>
#include <opencv2/nonfree/nonfree.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include <iostream>
#include <math.h>
#include <cfloat>
#include <fstream>




using namespace std;
using namespace cv;

namespace BOLD{
  
  enum BOLDRecognizerConstants{
   K_NEAREST_NEIGHBORS = 3,
  };
  
  class BOLDRecognizer{
  private:
    BOLDescriptor descriptor;
    vector<BOLDFeature*> trainedFeatures;
    
  public:
   ~BOLDRecognizer();
    BOLDRecognizer();
    BOLDDatum classify(BOLDFeature* f);
    BOLDDatum classify(BOLDDatum datum);
    void addLabeledFeature(BOLDFeature *f);
    void addLabeledFeatureFromFile(BOLDDatum datum);
    void addLabeledFeature(Mat &image,string label);
    void writeToFile(string fileName);
    void readFromFile(string fileName);
    void dialogue();
    void showAllFeatures();
    void clear();
  };
  
  
}

#endif