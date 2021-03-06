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
      datum.label = "";
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
        datum.label = ilabel;
	datum.hasDir = false;
        highestCount = 0;
        histBinSize = (2*M_PI)/HISTOGRAM_SIZE;

        
    }
    
    void BOLDFeature::setDatum(BOLDDatum dat)
    {
      datum = dat; 
    }
    
     BOLDFeature::BOLDFeature(double histo[HISTOGRAM_SIZE][HISTOGRAM_SIZE], int ientries, bool inormalized, BOLDDatum dat)
    {
        for(int i=0;i<HISTOGRAM_SIZE;i++) {
            for(int j=0;j<HISTOGRAM_SIZE;j++) {
                histogram[i][j] = histo[i][j];
            }
        }
        entries = ientries;
        normalized = inormalized;
        datum = dat;
	
        highestCount = 0;
        histBinSize = (2*M_PI)/HISTOGRAM_SIZE;

        
    }
    
    void BOLDFeature::normalize(){
      for(int i=0;i<HISTOGRAM_SIZE;i++) {
	for(int j=0;j<HISTOGRAM_SIZE;j++) {
	  histogram[i][j]/=entries;
	}
      }
    }
    
    bool BOLDFeature::isNormalized(){
     return normalized; 
    }
    
    double BOLDFeature::euclidDistanceFrom(BOLDFeature* f){
      double dist=0;
      double add;
      for(int i=0;i<HISTOGRAM_SIZE;i++){
	    for(int j=0;j<HISTOGRAM_SIZE;j++){
	      dist += (histogram[i][j]-f->getElement(i,j))*(histogram[i][j]-f->getElement(i,j));
	      
	    }
      }

      return dist;
    }
    
    double BOLDFeature::manhatDistanceFrom(BOLDFeature* f){
      double dist=0;
      double add;
      for(int i=0;i<HISTOGRAM_SIZE;i++){
	    for(int j=0;j<HISTOGRAM_SIZE;j++){
	      dist += abs(histogram[i][j]-f->getElement(i,j));
	      
	    }
      }

      return dist;
    }
    
    double BOLDFeature::varDistanceFrom(BOLDFeature* f,int power){
      double dist=0;
      double add;
      for(int i=0;i<HISTOGRAM_SIZE;i++){
	    for(int j=0;j<HISTOGRAM_SIZE;j++){
	      dist += pow(abs(histogram[i][j]-f->getElement(i,j)),power);
	      
	    }
      }

      return dist;
    }
    
    double BOLDFeature::canberraDistanceFrom(BOLDFeature* f){
      double dist=0;
      double add;
      double tmp;
      for(int i=0;i<HISTOGRAM_SIZE;i++){
	    for(int j=0;j<HISTOGRAM_SIZE;j++){
	      tmp = abs(histogram[i][j]-f->getElement(i,j))/(abs(histogram[i][j])+abs(f->getElement(i,j)));
	      if(tmp!=tmp)
		dist+=999999999.0;
	      else
		dist+=tmp;
	    }
      }
      if(dist!=dist) cout << "NAN!!!!\n";
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
	//cout << "alpha = " << alpha << "\n";
	alpha-=M_PI*2;
      }
      while(beta>=M_PI*2){
	//cout << "beta = " << beta << "\n";
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
    
    void BOLDFeature::setDir(string dir){
      datum.filename = dir;
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
      datum.label = l;
    }
    
    string BOLDFeature::getLabel(){
      return datum.label; 
    }
    
    BOLDDatum BOLDFeature::getDatum(){
     return datum; 
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
      output << datum.label << '\n';
      output << datum.filename << "\n";
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
