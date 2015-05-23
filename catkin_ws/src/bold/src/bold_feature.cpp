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
      histBinSize = (2*M_PI)/HISTOGRAM_SIZE;
      entries = 0;
      highestCount=0;
      normalized=false;
      label = "";
    }
    
    BOLDFeature::BOLDFeature(double histo[HISTOGRAM_SIZE][HISTOGRAM_SIZE], int ientries, bool inormalized, string ilabel)
    {
        for(int i=0;i<HISTOGRAM_SIZE;i++) {
            for(int j=0;j<HISTOGRAM_SIZE;j++) {
                histogram[i][j] = histo[i][j];
            }
        }
        entries = ientries;
        normalized = inormalized;
        label = ilabel;
        highestCount = 0;
        histBinSize = (2*M_PI)/HISTOGRAM_SIZE;

        
    }
    
    void BOLDFeature::normalize(){
      for(int i=0;i<HISTOGRAM_SIZE;i++) {
	for(int j=0;j<HISTOGRAM_SIZE;j++) {
	  histogram[i][j]/=entries;
	}
      }
      cout << entries << "entries \n";
    }
    
    bool BOLDFeature::isNormalized(){
     return normalized; 
    }
    
    double BOLDFeature::distanceFrom(BOLDFeature* f){
      double dist=0;
      double add;
      for(int i=0;i<HISTOGRAM_SIZE;i++){
	    for(int j=0;j<HISTOGRAM_SIZE;j++){
	      dist += (histogram[i][j]-f->getElement(i,j))*(histogram[i][j]-f->getElement(i,j));
	      
	    }
      }
      //returns squared distance
      return dist;
    }
    
    double BOLDFeature::getElement(int i,int j){
     return histogram[i][j]; 
    }
    
    int BOLDFeature::getMaxFeatureElement(){
     return  highestCount;
    }
    
    int BOLDFeature::getNEntries(){
     return entries; 
    }
    
    void BOLDFeature::add(double alpha,double beta){
      int a,b;
      
     // cout << "binsize = " << histBinSize << "\n";
      while(alpha>=M_PI*2){
	cout << "alpha = " << alpha << "\n";
	alpha-=M_PI*2;
      }
      while(beta>=M_PI*2){
	cout << "beta = " << beta << "\n";
	beta-=M_PI*2;
	
      }
      a = (int)(alpha/histBinSize);
      b= (int)(beta/histBinSize);
      
      histogram[a][b]++;
      if(histogram[a][b]>highestCount)
	highestCount = histogram[a][b];
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
	  image.data[i+j*FEATURE_SHOW_SIZE] =(uchar) (255*histogram[i/imBlockSize][j/imBlockSize]*entries/highestCount);
	}
      }
      cv::namedWindow("BOLD feature: "+name,cv::WINDOW_AUTOSIZE);
      imshow("BOLD feature: "+name,image);
    }
    
    void BOLDFeature::setLabel(string l){
      label = l;
    }
    
    string BOLDFeature::getLabel(){
      return label; 
    }


    void BOLDFeature::writeTo(std::ostream &output)
    {
        for(size_t r = 0; HISTOGRAM_SIZE != r; ++r)
        {
            for(size_t k = 0;HISTOGRAM_SIZE != k; ++k)
            {
                output << histogram[r][k] << ' ';
            }
            output << '\n';
        }

      output << entries << '\n';
      output << normalized << '\n';
      output << label << '\n';
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
