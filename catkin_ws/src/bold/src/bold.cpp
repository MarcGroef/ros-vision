#include "bold.hpp"


extern "C"{
  #include "lsd_1.6/lsd.h"
  #include "image_ext/image_ext.h"  
}


using namespace std;
using namespace cv;
using namespace BOLD;

namespace BOLD{
 

  
  
  
  //BOLD Feature functions
  
  BOLDFeature::BOLDFeature(){
   for(int i=0;i<HISTOGRAM_SIZE;i++) {
    for(int j=0;j<HISTOGRAM_SIZE;j++) {
      histogram[i][j]=0;
    }
   }
  }
  
  
  
  
  
  //BOLD Descriptor functions*/
  
  
  //Constructor
  BOLDescriptor::BOLDescriptor(){
    lines = NULL;
    imageIsSet = false;
    
  }
  
  
  //load an image into the BOLDescriptor
  void BOLDescriptor::setImage(Mat im){
    image = im;
    imageIsSet = true;
    histBinSize = M_PI/HISTOGRAM_SIZE;
  }
  
  //perform the LSD line detection from code of -von Gioi - e.a
  //Currently the line segments are written to a .eps image.
  
  void BOLDescriptor::detectLines(){
    double * im = char_to_image_double_ptr(image.cols,image.rows,(char*)image.data);
    
    int x,y;
    int X = image.cols;  // x image size 
    int Y = image.rows; //  y image size 

  
    // LSD call 
    lines = (Line*)lsd_scale(&nLines,im,X,Y,1.0);

    cout << nLines << " line segments found\n";
    write_eps((double*)lines,nLines,7,(char*)"BOLDLSDout.eps",X,Y,.1);
    cout << "Line image written to BOLDLSDout.eps..\n";
    // free memory 

    free( (void *) im );
  }
    
  void BOLDescriptor::describe(){
    int i,j;

    double alpha,beta;
    double gi,gj;
    int meanX,meanY;
    
    if(lines==NULL){
      if(!imageIsSet){
	cout << "BOLD::BOLDescriptor.describe() error: image not set.\n";
	exit(-1);
      }else
	detectLines();
    }
    
    for(i=0;i<nLines;i++){
      for(j=i+1;j<nLines;j++){
	//following variable namings from BOLD paper by Tombari e.a.
	gi = image.at<double>((int)((lines[i].x1+lines[i].x2))/2,(int)((lines[i].y1+lines[i].y2)/2));
	gj = image.at<double>((int)((lines[j].x1+lines[j].x2))/2,(int)((lines[j].y1+lines[j].y2)/2));
	
	
      }
    }
    
  }
  
  
 
  
}

int main(int argc,char**argv){
  ros::init(argc,argv,"bold");
  cv::initModule_nonfree();
  
  
  BOLDescriptor d;
  d.setImage(cv::imread(argv[1], CV_LOAD_IMAGE_GRAYSCALE));
  d.describe();
  
 
  return 0;
}