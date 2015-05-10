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
      HISTOGRAM_SIZE = 12,
      FEATURE_SHOW_SIZE = 300,
      
    };
    
    
    class BOLDFeature{
    private:
      double histogram[HISTOGRAM_SIZE][HISTOGRAM_SIZE];//  [alphas][betas]
      int entries;
      double histBinSize;
      int highestCount;
      bool normalized;
      
    public:
      
      BOLDFeature();
      void add(double alpha,double beta);
      double distanceFrom(BOLDFeature f);
      void print();
      void show(string name);
      
      int getNEntries();
      int getMaxFeatureElement();
      
      void normalize();
      void clear();
      
    };
  }

#endif