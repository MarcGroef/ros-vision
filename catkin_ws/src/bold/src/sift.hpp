#ifndef BOLD_SIFT_HPP
#define BOLD_SIFT_HPP

#include <opencv2/nonfree/nonfree.hpp>

#include "bold_datum.hpp"

using namespace cv;


namespace BOLD{
 

  
  class SIFTFeature{
    BOLDDatum datum;
    vector<KeyPoint> keypoints;
  public:
    SIFTFeature(BOLDDatum datum,vector<KeyPoint> keypoints);
    BOLDDatum getDatum();
    vector<KeyPoint>& getKeypoints();
  }
  
  class SIFTClassifier{
    vector<SIFTFeature> features;
    
  public:
    void train(BOLDDatum datum)
  }
  
}

#endif