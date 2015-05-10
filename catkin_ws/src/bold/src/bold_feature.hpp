#ifndef BOLD_FEATURE_H
#define BOLD_FEATURE_H

#include <math.h>
#include <iostream>

#include <opencv/cv.h>
#include <opencv2/nonfree/nonfree.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

  using namespace std;
  
  namespace BOLD{
    enum BOLDFeatureConstants{
      HISTOGRAM_SIZE = 10,
      FEATURE_SHOW_SIZE = 300,
      
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
    void show(string name);
    void clear();
    };
  }

#endif