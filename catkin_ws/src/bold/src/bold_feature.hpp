#ifndef BOLD_FEATURE_H
#define BOLD_FEATURE_H

#include <math.h>
#include <iostream>

#include <opencv/cv.h>
#include <opencv2/nonfree/nonfree.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

  using namespace std;
  using namespace cv;
  
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
      string label;
      
    public:
      
      BOLDFeature();
      BOLDFeature(double histo[HISTOGRAM_SIZE][HISTOGRAM_SIZE], int entries, bool normalized, string label);
      void add(double alpha,double beta);
      double distanceFrom(BOLDFeature f);
      void print();
      void show(string name);
      
      int getNEntries();
      int getMaxFeatureElement();
      
      double getElement(int i,int j);
      void normalize();
      void clear();

      void writeTo(std::ostream &output);
      
      void setLabel(string label);
      string getLabel();
      
      bool isNormalized();
      
    };
  }

#endif
