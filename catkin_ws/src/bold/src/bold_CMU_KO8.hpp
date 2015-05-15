#ifndef BOLD_CMU_KO8_HPP
#define BOLD_CMU_KO8_HPP



#include <opencv/cv.h>
#include <opencv2/nonfree/nonfree.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include <math.h>

#include <vector>
#include <iostream>
#include <fstream>
#include <string>
#include <cfloat>

#include "bold_recognizer.hpp"


using namespace cv;

namespace BOLD{
  enum CMU_KO8_Constants{
   CMU_N_TRAINING_ITEMS = 8, 
  };
  
class CMU_KO8_handler{
  private:
    string directory;
    BOLDRecognizer br;
    Mat convertMaskedImage(string image,string mask);
  public:
    CMU_KO8_handler();
    void train();
    void test();
  };
  
}


#endif