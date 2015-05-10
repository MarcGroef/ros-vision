#include "bold_feature.hpp"

  using namespace std;
  using namespace cv;

  namespace BOLD{
    
    
    BOLDFeature::BOLDFeature(){
      
    for(int i=0;i<HISTOGRAM_SIZE;i++) {
      for(int j=0;j<HISTOGRAM_SIZE;j++) {
	histogram[i][j]=0;
      }
    }
    histBinSize = 2*M_PI/HISTOGRAM_SIZE;
    entries = 0;
    highestCount=0;
    }
    

    
    void BOLDFeature::add(double alpha,double beta){
      histogram[(int)(alpha/histBinSize)][(int)(beta/histBinSize)]++;
      if(histogram[(int)(alpha/histBinSize)][(int)(beta/histBinSize)]>highestCount)
	highestCount = histogram[(int)(alpha/histBinSize)][(int)(beta/histBinSize)];
      entries++;
    }
    
    void BOLDFeature::print(){
      cout << "BOLD feature 2D Histogram with " << entries << " entries:\n" << "Highest entry is " << highestCount <<"\n";;
      
      for(int i=0;i<HISTOGRAM_SIZE;i++){
      for(int j=0;j<HISTOGRAM_SIZE;j++){
	cout << histogram[i][j] << ", ";
      }
      cout << "\n";
      }
      
    }
    
    void BOLDFeature::show(string name){
      Mat image(FEATURE_SHOW_SIZE,FEATURE_SHOW_SIZE,CV_8UC1);
      
      
      int imBlockSize = FEATURE_SHOW_SIZE/HISTOGRAM_SIZE;
      for(int i=0;i<FEATURE_SHOW_SIZE;i++){
      for(int j=0;j<FEATURE_SHOW_SIZE;j++) {
	image.data[i+j*FEATURE_SHOW_SIZE] =(uchar) (255*histogram[i/imBlockSize][j/imBlockSize]/highestCount);
      }
      }
      cv::namedWindow("BOLD feature: "+name,cv::WINDOW_AUTOSIZE);
      imshow("BOLD feature: "+name,image);
    }
    
    void BOLDFeature::clear(){
      for(int i=0;i<HISTOGRAM_SIZE;i++) {
	for(int j=0;j<HISTOGRAM_SIZE;j++) {
	  histogram[i][j]=0;
	}
      }
      entries = 0;
      highestCount=0;
    }
  }