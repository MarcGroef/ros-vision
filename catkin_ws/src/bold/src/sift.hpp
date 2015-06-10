#ifndef BOLD_SIFT_HPP
#define BOLD_SIFT_HPP

#include <opencv2/imgproc/imgproc.hpp>
#include <cv_bridge/cv_bridge.h>
#include <opencv2/opencv.hpp>
#include <opencv2/nonfree/nonfree.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/nonfree/features2d.hpp>
#include "opencv2/features2d/features2d.hpp"
#include <opencv2/legacy/legacy.hpp>
#include <opencv2/objdetect/objdetect.hpp>

#include "bold_datum.hpp"

#include <vector>
using namespace cv;
using namespace std;

namespace BOLD{
 
  struct Object
{
	cv::Mat image;
	vector<cv::KeyPoint> keypoints;
	cv::Mat descriptors;
	bool largeObject; // if true object is large, else object is small
	BOLDDatum datum;
};
  

  
  class SIFTClassifier{
    vector<Object> objects;
    
  public:
    void train(BOLDDatum datum);
    BOLDDatum classify(BOLDDatum d,bool isLarge);
    void clear();
  };
  
}

#endif