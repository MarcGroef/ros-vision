#ifndef BOLD_FEATURE_H
#define BOLD_FEATURE_H

#include <math.h>
#include <iostream>
#include <cmath>

#include <opencv/cv.h>
#include <opencv2/nonfree/nonfree.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include "bold_datum.hpp"

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
      
      BOLDDatum datum;
    public:
      
      BOLDFeature();
      BOLDFeature(double histo[HISTOGRAM_SIZE][HISTOGRAM_SIZE], int entries, bool normalized, string label);
      BOLDFeature(double histo[HISTOGRAM_SIZE][HISTOGRAM_SIZE], int ientries, bool inormalized, BOLDDatum datum);
      void add(double alpha,double beta);
      
      
      double euclidDistanceFrom(BOLDFeature* f);
      double manhatDistanceFrom(BOLDFeature* f);
      double canberraDistanceFrom(BOLDFeature* f);  //is crappy for this
      double varDistanceFrom(BOLDFeature* f,int power); //power = 1 : manhat, 2: euclid....
      
      void print();
      void show(string name);
      
      int getNEntries();
      int getMaxFeatureElement();
      
      double getElement(int i,int j);
      void normalize();
      void clear();

      void writeTo(std::ostream &output);
      void setDatum(BOLDDatum datum);
      void setLabel(string label);
      void setDir(string dir);
      BOLDDatum getDatum();
      string getLabel();
      
      bool isNormalized();
      
    };
  }

#endif
