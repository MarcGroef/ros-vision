#include "bold_CMU_KO8.hpp"

using namespace std;
using namespace cv;

namespace BOLD{
  
  
  
  CMU_KO8_handler::CMU_KO8_handler(){
    directory = "../../CMU_KO8/";
  }
  
  
  Mat CMU_KO8_handler::convertMaskedImage(string imageFile,string maskFile){
    cout << "reading: " << imageFile << " and " << maskFile << "\n";
    Mat image = cv::imread(imageFile, CV_LOAD_IMAGE_GRAYSCALE);
    Mat mask =  cv::imread(maskFile, CV_LOAD_IMAGE_GRAYSCALE);
    
    Mat maskedImage(mask);
    for (size_t i=0;i<mask.cols;i++)
      for(size_t j=0;j<mask.rows;j++)
	if( (int)mask.data[i+j*mask.cols] == 255){
	    mask.data[i+j*mask.cols] = image.data[i+j*mask.cols];
	}

      
    
    return mask;
  }
  
  void CMU_KO8_handler::train(){
    cout << "train\n";
    string singleDir = directory + "single/train/";
    string heights = singleDir+"heights.txt";
    
    string trainDir;
    
    std::ifstream input(heights.c_str(), std::ios::in);
    if(!input){
      cout << "error reading file..\n";
      if(input.eof()) cout << "end of file..";
      if(!input.is_open()) "file not open..\n";
      input.close();
    }
    cout << heights + "\n" ;
    
    string labels[CMU_N_TRAINING_ITEMS];
    string dump;
    for(int i=0;i<CMU_N_TRAINING_ITEMS;i++){
      input >> labels[i];
      input >> dump;
      cout << labels[i] << "\n";
    
      trainDir = singleDir+labels[i]+"/";
      
      Mat image = convertMaskedImage(trainDir+labels[i]+".jpg",trainDir+labels[i]+"_mask.png");
      br.addLabeledFeature(image,labels[i]);
      
    }
    input.close();
    
    
  }
  
  
  
}

