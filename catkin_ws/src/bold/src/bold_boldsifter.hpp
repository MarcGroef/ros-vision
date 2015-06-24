#ifndef BOLD_BOLDSIFTER_HPP
#define BOLD_BOLDSIFTER_HPP


#include "bold_recognizer.hpp"
#include "sift.hpp"
#include "bold_datum.hpp"

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

#include <vector>


using namespace std;
using namespace cv;

namespace BOLD{
  
    class BOLDSifter{
      BOLDRecognizer bold;
      SIFTClassifier sift;
      
      
    public:
      void train(BOLDDatum datum);
      BOLDDatum classify(BOLDDatum d);
      void clear();
    };
}

#endif
