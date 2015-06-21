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
#include <algorithm>
#include <vector>




using namespace std;
using namespace cv;

namespace BOLD{
  
  enum BOLDRecognizerConstants{
   K_NEAREST_NEIGHBORS = 3,  //use for classifying
   K_NEAREST_NEIGHBORS_MEMORY_SIZE = 10,
  };
  
  class BOLDRecognizer{
  private:
    BOLDescriptor descriptor;
    vector<BOLDFeature*> trainedFeatures;
    double distances[K_NEAREST_NEIGHBORS_MEMORY_SIZE];
    int kNearestNeighborIndices[K_NEAREST_NEIGHBORS_MEMORY_SIZE];
    int labelFrequencies[K_NEAREST_NEIGHBORS_MEMORY_SIZE];
    bool isMentLabel(BOLDDatum d,vector<BOLDDatum> labels);
    //bool larger(int i,int j);
  public:
   ~BOLDRecognizer();
    BOLDRecognizer();
    BOLDDatum classify(BOLDFeature* f);
    BOLDDatum classify(BOLDDatum datum);

    vector<BOLDDatum> getSortedLabels();
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